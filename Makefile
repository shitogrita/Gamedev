# Makefile wrapper for BrickGame v2.0
# Targets: all, install, uninstall, clean, dvi, dist, test, gcov_report

# ----------------------------------------------------------------------
# Configurable variables
# ----------------------------------------------------------------------
BUILD_DIR ?= build
CONFIG    ?= Release
CMAKE     ?= cmake

# CMake targets (у тебя они так называются)
EXEC_DESKTOP_TARGET ?= BrickGame_Desktop
EXEC_CLI_TARGET     ?= BrickGame_CLI
TEST_TARGET         ?= tests

# Install locations
INSTALL_DIR ?= ./bin
INSTALL_DESKTOP_NAME ?= BrickGame_Desktop
INSTALL_CLI_NAME     ?= BrickGame_CLI

# dist archive
DIST_DIR     ?= dist
DIST_NAME    ?= brickgame-$(shell date +%Y%m%d)
DIST_ARCHIVE ?= $(DIST_DIR)/$(DIST_NAME).tar.gz

# coverage
GCOV_DIR        := $(BUILD_DIR)/coverage
GCOV_INFO_FILE  := $(GCOV_DIR)/coverage.info

.PHONY: all configure install uninstall clean dvi dist test gcov_report help

# ----------------------------------------------------------------------
# Build
# ----------------------------------------------------------------------
all: configure
	$(CMAKE) --build $(BUILD_DIR) --config $(CONFIG)

configure:
	@mkdir -p $(BUILD_DIR)
	$(CMAKE) -S . -B $(BUILD_DIR) -DCMAKE_BUILD_TYPE=$(CONFIG)

# ----------------------------------------------------------------------
# Install / uninstall
# ----------------------------------------------------------------------
install: all
	@mkdir -p "$(INSTALL_DIR)"
	@cp "$(BUILD_DIR)/$(EXEC_DESKTOP_TARGET)" "$(INSTALL_DIR)/$(INSTALL_DESKTOP_NAME)"
	@cp "$(BUILD_DIR)/$(EXEC_CLI_TARGET)"     "$(INSTALL_DIR)/$(INSTALL_CLI_NAME)"
	@echo "Installed:"
	@echo "  $(INSTALL_DIR)/$(INSTALL_DESKTOP_NAME)"
	@echo "  $(INSTALL_DIR)/$(INSTALL_CLI_NAME)"

uninstall:
	@rm -f "$(INSTALL_DIR)/$(INSTALL_DESKTOP_NAME)" "$(INSTALL_DIR)/$(INSTALL_CLI_NAME)" || true
	@echo "Uninstalled binaries from $(INSTALL_DIR)"

# ----------------------------------------------------------------------
# Clean
# ----------------------------------------------------------------------
clean:
	@if [ -d "$(BUILD_DIR)" ]; then \
		$(CMAKE) --build $(BUILD_DIR) --target clean --config $(CONFIG) || true; \
	else \
		echo "$(BUILD_DIR) does not exist."; \
	fi

	rm -rf docs dist build cmake-*

# ----------------------------------------------------------------------
# Tests
# ----------------------------------------------------------------------
test: configure
	@echo "==> Building tests..."
	$(CMAKE) --build $(BUILD_DIR) --target $(TEST_TARGET) --config $(CONFIG)
	@echo "==> Running tests..."
	@cd $(BUILD_DIR)/Test && ./$(TEST_TARGET)

# ----------------------------------------------------------------------
# Coverage report
# ----------------------------------------------------------------------
gcov_report:
	@echo "==> Building with coverage flags..."
	@mkdir -p $(BUILD_DIR)
	$(CMAKE) -S . -B $(BUILD_DIR) \
		-DCMAKE_BUILD_TYPE=Debug \
		-DCMAKE_C_FLAGS="--coverage" \
		-DCMAKE_CXX_FLAGS="--coverage" \
		-DCMAKE_EXE_LINKER_FLAGS="--coverage" \
		-DCMAKE_SHARED_LINKER_FLAGS="--coverage"

	@echo "==> Building tests..."
	$(CMAKE) --build $(BUILD_DIR) --target $(TEST_TARGET)

	@echo "==> Running tests..."
	@cd $(BUILD_DIR)/Test && ./$(TEST_TARGET) || true

	@echo "==> Collecting coverage..."
	@mkdir -p $(GCOV_DIR)

	lcov --directory $(BUILD_DIR) --capture \
	     --output-file $(GCOV_INFO_FILE) \
	     --ignore-errors inconsistent,gcov,unsupported,format

	lcov --remove $(GCOV_INFO_FILE) \
	     "/usr/*" \
	     "/System/*" \
	     "/Library/*" \
	     "/opt/homebrew/*" \
	     "*/gtest/*" \
	     "*/gmock/*" \
	     "/usr/local/include/gtest/*" \
	     "*/src/Test/*" \
	     "*/Test/*" \
	     "*/tests/*" \
	     "*/CTestTestfile.cmake" \
	     "*/CMakeFiles/*" \
	     --output-file $(GCOV_INFO_FILE) \
	     --ignore-errors inconsistent,gcov,unsupported,format,unused


	@echo "==> Generating HTML report..."
	genhtml $(GCOV_INFO_FILE) --output-directory $(GCOV_DIR) \
	        --ignore-errors inconsistent,gcov,unsupported,format,category,unused

	@echo "Report at: $(GCOV_DIR)/index.html"

# ----------------------------------------------------------------------
# dvi (docs placeholder)
# ----------------------------------------------------------------------
dvi:
	@echo "==> Generating documentation for Snake (Doxygen + LaTeX/HTML)"
	@if ! command -v doxygen >/dev/null 2>&1; then \
		echo "doxygen not found. Install doxygen to build documentation."; \
		exit 0; \
	fi
	@mkdir -p docs/snake
	@doxygen Doxyfile_snake

	@if [ -d docs/snake/latex ]; then \
		if command -v pdflatex >/dev/null 2>&1; then \
			echo "==> Building LaTeX docs (refman.pdf)..."; \
			$(MAKE) -C docs/snake/latex || true; \
		else \
			echo "pdflatex not found, skipping PDF/LaTeX build."; \
		fi \
	fi
	@echo "Documentation generated:"
	@echo "  HTML: docs/snake/html/index.html"
	@echo "  (PDF in docs/snake/latex/refman.pdf if LaTeX установлен)"

# ----------------------------------------------------------------------
# dist (source tarball)
# ----------------------------------------------------------------------
dist:
	@mkdir -p $(DIST_DIR)
	@cd .. && tar \
		--exclude=$(notdir $(BUILD_DIR)) \
		--exclude=$(notdir $(DIST_DIR)) \
		--exclude=$(notdir $(DIST_ARCHIVE)) \
		--exclude-vcs \
		-czf src/$(DIST_ARCHIVE) src
	@echo "Created archive: $(DIST_ARCHIVE)"


# ----------------------------------------------------------------------
# Help
# ----------------------------------------------------------------------
help:
	@echo "Available targets:"
	@echo "  all         - build project"
	@echo "  install     - install CLI/Desktop binaries"
	@echo "  uninstall   - remove installed binaries"
	@echo "  clean       - clean build directory"
	@echo "  test        - build and run tests"
	@echo "  gcov_report - build, run tests, generate coverage report"
	@echo "  dist        - create source tar.gz"
	@echo "  dvi         - placeholder"
