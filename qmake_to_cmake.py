#!/usr/bin/env python3
# -*- coding: utf-8 -*-

import argparse
import os
import re
from pathlib import Path
from dataclasses import dataclass, field
from typing import Dict, List, Optional, Tuple


VAR_RE = re.compile(r'^\s*([A-Za-z0-9_]+)\s*([+:\-]?=)\s*(.*)$')
COMMENT_RE = re.compile(r'(?<!\\)#.*$')
CONT_RE = re.compile(r'\\\s*$')


def strip_comment(line: str) -> str:
    # remove comments that start with # but keep escaped \#
    return COMMENT_RE.sub('', line).rstrip()


def tokenize_values(v: str) -> List[str]:
    # naive split (qmake can be more complex, but works for most projects)
    v = v.strip()
    if not v:
        return []
    return [x for x in re.split(r'\s+', v) if x]


@dataclass
class QmakeProject:
    pro_path: Path
    variables: Dict[str, List[str]] = field(default_factory=dict)
    ops: Dict[str, List[Tuple[str, str]]] = field(default_factory=dict)  # var -> [(op, value)]
    template: Optional[str] = None
    target: Optional[str] = None

    def add(self, var: str, op: str, values: List[str]):
        self.ops.setdefault(var, [])
        for v in values:
            self.ops[var].append((op, v))

        cur = self.variables.get(var, [])
        if op in ('=', ':='):
            cur = values[:]
        elif op == '+=':
            cur.extend(values)
        elif op == '-=':
            cur = [x for x in cur if x not in values]
        else:
            # treat unknown as add
            cur.extend(values)
        self.variables[var] = cur


def read_qmake_file(path: Path, visited: set) -> List[str]:
    path = path.resolve()
    if path in visited:
        return []
    visited.add(path)

    if not path.exists():
        return []

    lines: List[str] = []
    raw = path.read_text(encoding='utf-8', errors='ignore').splitlines()

    buf = ""
    for ln in raw:
        ln = strip_comment(ln)
        if not ln.strip():
            continue

        if buf:
            buf += " " + ln.strip()
        else:
            buf = ln.strip()

        if CONT_RE.search(ln):
            buf = CONT_RE.sub('', buf).rstrip()
            continue

        lines.append(buf)
        buf = ""

    # handle includes: include(file.pri)
    expanded: List[str] = []
    inc_re = re.compile(r'^\s*include\(\s*([^)]+?)\s*\)\s*$')
    for line in lines:
        m = inc_re.match(line)
        if m:
            inc = m.group(1).strip().strip('"').strip("'")
            inc_path = (path.parent / inc).resolve()
            expanded.extend(read_qmake_file(inc_path, visited))
        else:
            expanded.append(line)

    return expanded


def parse_qmake_project(pro_path: Path) -> QmakeProject:
    p = QmakeProject(pro_path=pro_path.resolve())
    visited = set()
    lines = read_qmake_file(p.pro_path, visited)

    for line in lines:
        m = VAR_RE.match(line)
        if not m:
            # ignore scopes/conditions for now (win32:, unix:, contains(), etc.)
            continue
        var, op, rhs = m.group(1), m.group(2), m.group(3)
        vals = tokenize_values(rhs)
        p.add(var, op, vals)

    p.template = (p.variables.get('TEMPLATE', [None])[0]) if 'TEMPLATE' in p.variables else None
    p.target = (p.variables.get('TARGET', [None])[0]) if 'TARGET' in p.variables else None
    return p


def cmake_quote_list(items: List[str]) -> str:
    # keep relative paths as-is
    return "\n    ".join(items)


def guess_qt_components(qt_plus: List[str]) -> List[str]:
    # Map qmake 'QT +=' items to CMake components
    mapping = {
        "core": "Core",
        "gui": "Gui",
        "widgets": "Widgets",
        "qml": "Qml",
        "quick": "Quick",
        "quickcontrols2": "QuickControls2",
        "network": "Network",
        "sql": "Sql",
        "xml": "Xml",
        "testlib": "Test",
        "concurrent": "Concurrent",
        "printsupport": "PrintSupport",
        "svg": "Svg",
        "multimedia": "Multimedia",
        "opengl": "OpenGL",
    }
    out = []
    for x in qt_plus:
        k = x.strip().lower()
        if k in mapping and mapping[k] not in out:
            out.append(mapping[k])
    if not out:
        out = ["Core"]
    return out


def write_file(path: Path, content: str, dry_run: bool):
    if dry_run:
        print(f"[dry-run] would write: {path}")
        return
    path.parent.mkdir(parents=True, exist_ok=True)
    path.write_text(content, encoding="utf-8")
    print(f"written: {path}")


def generate_module_cmakelists(qp: QmakeProject, qt_major: int) -> str:
    vars = qp.variables
    template = qp.template or "app"
    target = qp.target or qp.pro_path.stem

    sources = vars.get("SOURCES", [])
    headers = vars.get("HEADERS", [])
    forms = vars.get("FORMS", [])
    resources = vars.get("RESOURCES", [])
    include_paths = vars.get("INCLUDEPATH", [])
    defines = vars.get("DEFINES", [])
    qt_plus = vars.get("QT", [])
    libs = vars.get("LIBS", [])

    qt_components = guess_qt_components(qt_plus)

    is_test = (template == "app" and ("testlib" in [x.lower() for x in qt_plus] or "testcase" in [x.lower() for x in vars.get("CONFIG", [])]))
    # qmake tests often are TEMPLATE = app + QT += testlib
    # libs: keep raw, best-effort convert -lfoo to foo later (TODO)
    raw_libs = " ".join(libs).strip()

    # Qt6: qt_add_executable/qt_add_library with AUTOMOC/UIC/RCC
    # Qt5: still works with find_package(Qt5 ...) and target_link_libraries Qt5::Core etc.
    qt_pkg = "Qt6" if qt_major == 6 else "Qt5"
    qt_ns = "Qt6::" if qt_major == 6 else "Qt5::"

    target_type = "add_library" if template == "lib" else "add_executable"
    cmake = []
    cmake.append(f"# Generated from {qp.pro_path.name}")
    cmake.append("")
    cmake.append("set(CMAKE_AUTOMOC ON)")
    cmake.append("set(CMAKE_AUTOUIC ON)")
    cmake.append("set(CMAKE_AUTORCC ON)")
    cmake.append("")
    cmake.append(f"{target_type}({target}")
    if sources:
        cmake.append("    " + cmake_quote_list(sources))
    if headers:
        cmake.append("    " + cmake_quote_list(headers))
    if forms:
        cmake.append("    " + cmake_quote_list(forms))
    if resources:
        cmake.append("    " + cmake_quote_list(resources))
    cmake.append(")")
    cmake.append("")

    if include_paths:
        cmake.append(f"target_include_directories({target} PRIVATE")
        cmake.append("    " + cmake_quote_list(include_paths))
        cmake.append(")")
        cmake.append("")

    if defines:
        defs = "\n    ".join([f'"{d}"' for d in defines])
        cmake.append(f"target_compile_definitions({target} PRIVATE")
        cmake.append(f"    {defs}")
        cmake.append(")")
        cmake.append("")

    # Link Qt
    cmake.append(f"target_link_libraries({target} PRIVATE")
    for c in qt_components:
        cmake.append(f"    {qt_ns}{c}")
    cmake.append(")")
    cmake.append("")

    # Raw libs passthrough (TODO: parse better)
    if raw_libs:
        cmake.append(f"# TODO: review qmake LIBS conversion: {raw_libs}")
        cmake.append("")

    if is_test:
        cmake.append("add_test(NAME " + target + " COMMAND " + target + ")")
        cmake.append("")

    return "\n".join(cmake)


def generate_root_cmakelists(root: Path, subdirs: List[Path], qt_major: int, project_name: str) -> str:
    qt_pkg = "Qt6" if qt_major == 6 else "Qt5"
    cmake = []
    cmake.append("cmake_minimum_required(VERSION 3.16)")
    cmake.append(f'project({project_name} LANGUAGES CXX)')
    cmake.append("")
    cmake.append("set(CMAKE_CXX_STANDARD 17)")
    cmake.append("set(CMAKE_CXX_STANDARD_REQUIRED ON)")
    cmake.append("")
    cmake.append("include(CTest)")
    cmake.append("")
    cmake.append(f"find_package({qt_pkg} REQUIRED COMPONENTS Core Gui Widgets Test Qml Quick QuickControls2 Network Sql Xml)")
    cmake.append("# Note: components above are a superset; you can trim later.")
    cmake.append("")
    for sd in subdirs:
        rel = sd.as_posix()
        cmake.append(f"add_subdirectory({rel})")
    cmake.append("")
    return "\n".join(cmake)


def main():
    ap = argparse.ArgumentParser(description="Best-effort qmake (.pro/.pri) to CMake converter for Qt multi-module projects.")
    ap.add_argument("root", type=str, help="Project root directory containing a top-level .pro (SUBDIRS) or multiple .pro files.")
    ap.add_argument("--qt", type=int, choices=[5, 6], default=6, help="Qt major version for generated CMake (default: 6).")
    ap.add_argument("--top-pro", type=str, default="", help="Optional path to top-level .pro (relative to root).")
    ap.add_argument("--project-name", type=str, default="MigratedProject", help="CMake project() name.")
    ap.add_argument("--dry-run", action="store_true", help="Do not write files, only print what would be generated.")
    args = ap.parse_args()

    root = Path(args.root).resolve()
    if not root.exists():
        raise SystemExit(f"Root does not exist: {root}")

    # Find top .pro
    top_pro: Optional[Path] = None
    if args.top_pro:
        top_pro = (root / args.top_pro).resolve()
    else:
        # heuristics: prefer root/*.pro, else first found
        candidates = list(root.glob("*.pro"))
        top_pro = candidates[0].resolve() if candidates else None

    if not top_pro or not top_pro.exists():
        raise SystemExit("Could not find top .pro. Provide --top-pro.")

    top = parse_qmake_project(top_pro)
    subdir_entries = top.variables.get("SUBDIRS", [])

    sub_projects: List[Path] = []

    # SUBDIRS can contain names; attempt to locate matching .pro in subdir
    for sd in subdir_entries:
        sd_path = (top_pro.parent / sd).resolve()
        if sd_path.is_dir():
            pros = list(sd_path.glob("*.pro"))
            if pros:
                sub_projects.append(pros[0].resolve())
            else:
                # maybe the SUBDIR itself is a .pro name
                if sd.endswith(".pro") and sd_path.exists():
                    sub_projects.append(sd_path)
        else:
            # if entry is a .pro file
            if sd.endswith(".pro"):
                p = (top_pro.parent / sd).resolve()
                if p.exists():
                    sub_projects.append(p)

    # If no SUBDIRS, fallback: all *.pro under root (excluding top)
    if not sub_projects:
        all_pro = [p.resolve() for p in root.rglob("*.pro")]
        sub_projects = [p for p in all_pro if p != top_pro]

    if not sub_projects:
        raise SystemExit("No sub projects found. Check SUBDIRS or project layout.")

    # Generate per-subproject CMakeLists next to .pro
    add_subdirs: List[Path] = []
    for pro in sub_projects:
        qp = parse_qmake_project(pro)
        cmake_txt = generate_module_cmakelists(qp, args.qt)
        out = pro.parent / "CMakeLists.txt"
        write_file(out, cmake_txt, args.dry_run)
        add_subdirs.append(out.parent.relative_to(root))

    # Root CMakeLists
    root_cmake = generate_root_cmakelists(root, add_subdirs, args.qt, args.project_name)
    write_file(root / "CMakeLists.txt", root_cmake, args.dry_run)

    print("\nDone.")
    print("Next steps:")
    print("  - Review generated CMakeLists for LIBS / platform scopes.")
    print("  - Run: cmake -S . -B build && cmake --build build")
    print("  - Run tests: ctest --test-dir build")


if __name__ == "__main__":
    main()
