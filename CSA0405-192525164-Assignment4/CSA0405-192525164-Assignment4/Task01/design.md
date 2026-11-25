# Task 1 - File-System Design Review

## Why the proposed design fails
A single flat directory for 12,000 students, student-chosen file names,
contiguous allocation and no access control is not suitable for a college
file server: identical names such as `report.pdf` collide, one huge
directory is hard to search and manage, contiguous files that grow need
relocation and free space fragments, and without permissions any student
can read, change or delete another student's work.

## Improved design - hierarchical directory structure
```
CollegeServer/
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
  Backup/                     (administrator only)
```

## Naming rules
- The register number (students) or staff ID is the unique account name.
- File names carry course/project and version, e.g. `CSA0405_Assignment4_v1.pdf`.
- Avoid spaces and ambiguous names such as `final`, `new` or `report`.

## File attributes kept for every file
| Attribute | Purpose |
|---|---|
| Owner / group | Identifies the student and the course/team |
| Size and timestamps | Track storage use and changes |
| Type and extension | Identify the file format |
| ACL / permission bits | Grant read, write and execute rights |
| Checksum | Detect accidental corruption |

## Permissions
| Folder / file | Mode | Meaning |
|---|---|---|
| Private student directory | 0700 | Owner only |
| Private file | 0600 | Owner read/write only |
| Shared project directory | 2770 | Setgid group folder - group ownership is inherited by new files |
| Course material (read-only) | 0755 / 0644 | Readable by all, writable by owner |

Sharing is done with group ACLs on SharedProjects folders only, so team
members get the access they need without opening private folders.
Administrators manage quotas, auditing and backups, keep an off-server
copy, and test restoration regularly.
