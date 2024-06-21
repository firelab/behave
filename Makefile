# Set the build directory
BUILD_DIR := build

# Set the source directory
SRC_DIR := src

# CMake configuration
CMAKE_CMD := cmake

# Doxygen configuration
DOXYGEN_CMD := doxygen
DOCS_DIR := docs

# Tests
MORTALITY_TEST_DIR := src/testMortality/

# Build and clean targets
compile: $(BUILD_DIR)
	$(CMAKE_CMD) -B $(BUILD_DIR)
	$(CMAKE_CMD) --build $(BUILD_DIR)

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

clean:
	rm -rf $(BUILD_DIR) $(DOCS_DIR)

test:
	./build/testBehave

test_mortality:
	./build/testMortality $(MORTALITY_TEST_DIR)/FOFEM_input.tre $(MORTALITY_TEST_DIR)/FOFEM_Mortality_Output.csv results.csv

# Generate Docs
gendocs:
	$(DOXYGEN_CMD) Doxyfile

# Generate tags
gentags:
	find . -type f \( -name "*.cpp" -o -name "*.h" \) | etags -


# Default target
all: compile

# Develop target
dev: gendocs gentags

.PHONY: clean gendocs gentags test
