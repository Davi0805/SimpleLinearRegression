BUILD_DIR := build
JOBS ?= $(shell nproc 2>/dev/null || echo 4)

.PHONY: build run test rebuild full-build clean

build:
	cmake -S . -B $(BUILD_DIR)
	cmake --build $(BUILD_DIR) -j$(JOBS)

run: build
	./$(BUILD_DIR)/linear_regression

test: build
	ctest --test-dir $(BUILD_DIR) --output-on-failure

rebuild:
	cmake --build $(BUILD_DIR) --clean-first -j$(JOBS)

full-build: clean build

clean:
	rm -rf $(BUILD_DIR)
