# Linear Regression in C++

## Project Structure

```
linear_regression/
├── CMakeLists.txt
├── Makefile            # convenience wrapper: build/run/test/rebuild/full-build
├── README.md
├── src/
│   └── main.cpp        # your source files
├── include/            # your own headers
├── tests/              # GoogleTest suite (CMakeLists.txt + test_main.cpp)
├── data/               # CSV datasets
└── build/              # cmake build output (gitignore this)
```

## Build

A partir da raiz do projeto (usa `build/` internamente):

```bash
make build        # configura + compila
make run          # compila e executa ./build/linear_regression
make test         # compila e roda os testes (ctest)
make rebuild      # recompila (clean + build), mantendo o cache do cmake
make full-build   # apaga build/ e refaz tudo do zero
make clean        # apaga build/
```

Ou, manualmente, dentro de `build/`:

```bash
cd build
cmake ..
cmake --build .
./linear_regression
cmake --build . --target run   # equivalente a compilar + rodar
ctest                          # roda os testes
```

## Tests

Os testes usam [GoogleTest](https://github.com/google/googletest) (baixado via `FetchContent`, igual ao rapidcsv) e ficam em `tests/`. O executável de teste é `linear_regression_tests` e é registrado no CTest via `gtest_discover_tests`.

---

## Using rapidcsv

The project uses [rapidcsv](https://github.com/d99kris/rapidcsv) — a single-header, header-only library. CMake fetches it automatically on first build.

### Include

```cpp
#include "rapidcsv.h"
```

---

### Loading a CSV file

Given `data/dataset.csv`:

```
x,y
1.0,2.1
2.0,4.0
3.0,5.9
```

```cpp
rapidcsv::Document doc("../data/dataset.csv");
```

By default rapidcsv expects a header row on row 0 and no index column.

---

### Reading columns

```cpp
// by column name (requires header row)
std::vector<double> x = doc.GetColumn<double>("x");
std::vector<double> y = doc.GetColumn<double>("y");

// by column index (0-based)
std::vector<double> x = doc.GetColumn<double>(0);
```

---

### Reading rows

```cpp
// get row 0 as a tuple-like struct
std::vector<std::string> row = doc.GetRow<std::string>(0);

// get a single cell: GetCell<T>(col, row)
double val = doc.GetCell<double>(0, 1); // col 0, row 1
```

---

### Document options

```cpp
// no header row, no index column (both default to 0 = present)
rapidcsv::Document doc(
    "../data/dataset.csv",
    rapidcsv::LabelParams(-1, -1)   // -1 means "no label"
);

// custom separator (default is ',')
rapidcsv::Document doc(
    "../data/dataset.tsv",
    rapidcsv::LabelParams(),
    rapidcsv::SeparatorParams('\t')
);
```

---

### Full example

```cpp
#include <iostream>
#include <vector>
#include "rapidcsv.h"

int main() {
    rapidcsv::Document doc("../data/dataset.csv");

    std::vector<double> x = doc.GetColumn<double>("x");
    std::vector<double> y = doc.GetColumn<double>("y");

    for (size_t i = 0; i < x.size(); ++i) {
        std::cout << "x=" << x[i] << "  y=" << y[i] << "\n";
    }
}
```

---

### Common pitfalls

| Problem | Fix |
|---|---|
| File not found | Path is relative to where the binary runs (`build/`), so use `../data/file.csv` |
| Wrong type conversion | Make sure the template type matches the column content exactly |
| Extra whitespace in header | rapidcsv matches column names literally — trim your CSV headers |
| Missing values | Use `rapidcsv::ConverterParams(true)` to replace bad cells with a default |
# SimpleLinearRegression
