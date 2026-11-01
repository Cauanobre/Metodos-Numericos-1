CXX      = g++
CXXFLAGS = -Isrc -Wall -Wextra -Wpedantic -std=c++17 -O2
SRC      = src/main.cpp
BIN      = bin/main
DIST     = Equipe_MN1_Tema2

VALORES_A = 8\n0.1\n0.5\n1\n1.5\n2\n3\n5\n10\n1e-4\n

.PHONY: all static run clean dist csv graficos

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

# Roda o programa com vários valores de a (item g) e gera os CSVs em resultados/
csv: all
	mkdir -p resultados
	cd resultados && printf "$(VALORES_A)" | ../$(BIN) > /dev/null

# Gráficos e quadros em imagem (precisa de python3 + matplotlib)
graficos: csv
	python3 scripts/graficos.py

clean:
	rm -rf bin resultados/*.csv graficos/*.png $(DIST) $(DIST).zip

# Arquivo compactado final: apresentação, código fonte e executável estático
dist: static
	rm -rf $(DIST) $(DIST).zip
	mkdir -p $(DIST)/bin
	cp -r src makefile README.md $(DIST)/
	cp $(BIN) $(DIST)/bin/
	cp slides/*.pdf slides/*.pptx $(DIST)/ 2>/dev/null || echo "AVISO: nenhuma apresentacao encontrada em slides/"
	zip -r $(DIST).zip $(DIST)
	rm -rf $(DIST)
