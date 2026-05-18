CXX = g++

CXXFLAGS = -std=c++11 -Wall -Wextra -I$(SRC_DIR)

LDFLAGS = -lssl -lcrypto

SRC_DIR = src
BUILD_DIR = build

# =========================
# FUENTES DEL MONITOR
# =========================

MONITOR_SRCS = \
	$(SRC_DIR)/main.cpp \
	$(SRC_DIR)/enumerator.cpp \
	$(SRC_DIR)/elf_analyzer.cpp \
	$(SRC_DIR)/hash_engine.cpp \
	$(SRC_DIR)/scorer.cpp \
	$(SRC_DIR)/logger.cpp

# =========================
# FUENTES DEL INJECTOR
# =========================

INJECT_SRCS = \
	$(SRC_DIR)/injector_main.cpp \
	$(SRC_DIR)/ptrace_ops.cpp

# =========================
# BINARIOS
# =========================

MONITOR_BIN_RAW = $(BUILD_DIR)/simulador_raw
MONITOR_BIN_SYM = $(BUILD_DIR)/simulador_sys

STAGER_BIN_RAW = $(BUILD_DIR)/stager_raw
STAGER_BIN_SYM = $(BUILD_DIR)/stager_sys

INJECT_BIN_RAW = $(BUILD_DIR)/inject_raw
INJECT_BIN_SYM = $(BUILD_DIR)/inject_sys

SPOOF_BIN_RAW = $(BUILD_DIR)/spoof_raw
SPOOF_BIN_SYM = $(BUILD_DIR)/spoof_sys

TARGET_BIN_RAW = $(BUILD_DIR)/target_raw
TARGET_BIN_SYM = $(BUILD_DIR)/target_sys

# =========================
# TARGETS
# =========================

.PHONY: all clean monitor offensive

all: monitor offensive

monitor: $(MONITOR_BIN_RAW) $(MONITOR_BIN_SYM)

offensive: \
	$(STAGER_BIN_RAW) \
	$(STAGER_BIN_SYM) \
	$(INJECT_BIN_RAW) \
	$(INJECT_BIN_SYM) \
	$(SPOOF_BIN_RAW) \
	$(SPOOF_BIN_SYM) \
	$(TARGET_BIN_RAW) \
	$(TARGET_BIN_SYM)

# =========================
# MONITOR
# =========================

$(MONITOR_BIN_RAW): $(MONITOR_SRCS)
	@mkdir -p $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -s -o $@ $^ $(LDFLAGS)
	@echo "[OK] Monitor RAW compilado"

$(MONITOR_BIN_SYM): $(MONITOR_SRCS)
	@mkdir -p $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -g -o $@ $^ $(LDFLAGS)
	@echo "[OK] Monitor SYM compilado"

# =========================
# STAGER
# =========================

$(STAGER_BIN_RAW): $(SRC_DIR)/stager.cpp
	@mkdir -p $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -s -o $@ $<
	@echo "[OK] Stager RAW compilado"

$(STAGER_BIN_SYM): $(SRC_DIR)/stager.cpp
	@mkdir -p $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -g -o $@ $<
	@echo "[OK] Stager SYM compilado"

# =========================
# INJECTOR
# =========================

$(INJECT_BIN_RAW): $(INJECT_SRCS)
	@mkdir -p $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -s -o $@ $^
	@echo "[OK] Injector RAW compilado"

$(INJECT_BIN_SYM): $(INJECT_SRCS)
	@mkdir -p $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -g -o $@ $^
	@echo "[OK] Injector SYM compilado"

# =========================
# SPOOF
# =========================

$(SPOOF_BIN_RAW): $(SRC_DIR)/spoof.cpp
	@mkdir -p $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -s -o $@ $<
	@echo "[OK] Spoof RAW compilado"

$(SPOOF_BIN_SYM): $(SRC_DIR)/spoof.cpp
	@mkdir -p $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -g -o $@ $<
	@echo "[OK] Spoof SYM compilado"

# =========================
# TARGET
# =========================

$(TARGET_BIN_RAW): $(SRC_DIR)/target.cpp
	@mkdir -p $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -s -o $@ $<
	@echo "[OK] Target RAW compilado"

$(TARGET_BIN_SYM): $(SRC_DIR)/target.cpp
	@mkdir -p $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -g -o $@ $<
	@echo "[OK] Target SYM compilado"

# =========================
# CLEAN
# =========================

clean:
	rm -rf $(BUILD_DIR)
	@echo "[OK] Build eliminada"