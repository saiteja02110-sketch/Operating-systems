#!/usr/bin/env python3
"""Print the improved college file-server design for Task 1."""

TREE = """CollegeServer/
  Students/
    192525164/
      Documents/
      Assignments/
      Projects/
    <register-number>/        (one private folder per student)
  Staff/
    <staff-id>/
  Courses/
    <course-code>/<semester>/
  SharedProjects/
    <project-id>/             (group access only)
  Backup/                     (administrator only)"""

PERMISSIONS = [
    ("Private student directory", "0700", "owner only"),
    ("Private file", "0600", "owner read/write only"),
    ("Shared project directory", "2770", "setgid group folder, group ownership inherited"),
    ("Course material directory", "0755", "readable by all, writable by owner"),
    ("Course material file", "0644", "readable by all, writable by owner"),
]

def main():
    print("Task 1 - Improved file-system design")
    print()
    print("Directory structure:")
    print(TREE)
    print()
    print("Permissions:")
    for name, mode, meaning in PERMISSIONS:
        print(f"  {mode}  {name:<28} {meaning}")
    print()
    print("Naming: register number is the unique account name; files carry")
    print("course/project and version, e.g. CSA0405_Assignment4_v1.pdf")
    print("Sharing: group ACLs on SharedProjects only; private folders stay 0700.")

if __name__ == "__main__":
    main()
