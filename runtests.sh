#!/bin/bash
set -e

# 1. Generowanie plików projektu (jeśli folder 'build' nie istnieje lub się zmienił)
cmake -B build

# 2. Kompilacja wielowątkowa (wykorzystaj pełną moc procesora w WSL)
cmake --build build --parallel $(nproc)

# 3. Uruchomienie testów z wyświetlaniem błędów na żywo
ctest --test-dir build --output-on-failure