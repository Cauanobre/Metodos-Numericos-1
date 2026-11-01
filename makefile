CXX      = g++
CXXFLAGS = -Isrc -Wall -Wextra -Wpedantic -std=c++17 -O2
SRC      = src/main.cpp
BIN      = bin/main
DIST     = Equipe_MN1_Tema2

.PHONY: all static run clean dist

# Build de desenvolvimento (dinâmico)
all:
	mkdir -p bin
	$(CXX) $(CXXFLAGS) -o $(BIN) $(SRC)

# Build de entrega: executável estático (sem dependência de LIBs/DLLs)
static:
	mkdir -p bin
	$(CXX) $(CXXFLAGS) -static -o $(BIN) $(SRC)

# Executa dentro de resultados/, onde ficam os CSVs gerados
run: all
	mkdir -p resultados
	cd resultados && ../$(BIN)

clean:
	rm -rf bin resultados/*.csv $(DIST) $(DIST).zip

# Arquivo compactado final: apresentação, código fonte e executável estático
dist: static
	rm -rf $(DIST) $(DIST).zip
	mkdir -p $(DIST)/bin
	cp -r src makefile README.md $(DIST)/
	cp $(BIN) $(DIST)/bin/
	cp slides/*.pdf slides/*.pptx $(DIST)/ 2>/dev/null || echo "AVISO: nenhuma apresentacao encontrada em slides/"
	zip -r $(DIST).zip $(DIST)
	rm -rf $(DIST)
