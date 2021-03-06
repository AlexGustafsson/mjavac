# Disable echoing of commands
MAKEFLAGS += --silent

OUTPUT_PATH=build/production

FLEX_FLAGS=
BISON_FLAGS=-t

ifdef DEBUG
OUTPUT_PATH=build/debug
FLEX_FLAGS=--debug
BISON_FLAGS=-v -t --graph
endif

CPPFLAGS += -I . -I $(OUTPUT_PATH)

ast_source := $(shell find ast -type f -name "*.cc")
ast_objects := $(addprefix $(OUTPUT_PATH)/,$(ast_source:.cc=.o))
headers := $(shell find * -type f -name "*.hpp" -not -path "build/*")
includes := $(addprefix $(OUTPUT_PATH)/include/mjavac/,$(headers))

.PHONY: lib include clean

all: lib include

include: $(includes) $(OUTPUT_PATH)/parser.tab.hpp $(OUTPUT_PATH)/location.hh $(OUTPUT_PATH)/FlexLexer.h
	cp $(OUTPUT_PATH)/parser.tab.hpp $(OUTPUT_PATH)/location.hh $(OUTPUT_PATH)/FlexLexer.h $(OUTPUT_PATH)/include/mjavac/

lib: $(OUTPUT_PATH)/lib/mjavac/libmjavacparser.a

# Build a static library for the parser
$(OUTPUT_PATH)/lib/mjavac/libmjavacparser.a: $(ast_objects) $(OUTPUT_PATH)/parser.tab.o $(OUTPUT_PATH)/lexer.yy.o $(OUTPUT_PATH)/source.o
	mkdir -p $(dir $@)
ifeq ($(shell uname),Darwin)
	libtool -static -o $@ $^
else
	$(AR) rcs $@ $^
endif

# Copy headers
$(includes): $(OUTPUT_PATH)/include/mjavac/%.hpp: %.hpp
	mkdir -p $(dir $@)
	cp $< $@

# Compile ast
$(ast_objects): $(OUTPUT_PATH)/%.o: %.cc %.hpp
	mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) $(LDFLAGS) -c $< -o $@

# Compile the source
$(OUTPUT_PATH)/source.o: source.cc source.hpp
	mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) $(LDFLAGS) -c $< -o $@

# Compile the lexer
$(OUTPUT_PATH)/lexer.yy.o: $(OUTPUT_PATH)/parser.tab.hpp $(OUTPUT_PATH)/lexer.yy.cc
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) $(LDFLAGS) -c $(OUTPUT_PATH)/lexer.yy.cc -o $(OUTPUT_PATH)/lexer.yy.o

# Generate the lexer
# Expects flex 2.6 (brew install flex)
$(OUTPUT_PATH)/lexer.yy.cc: lexer.l scanner.hpp
	mkdir -p $(OUTPUT_PATH)
	flex $(FLEX_FLAGS) --outfile $(OUTPUT_PATH)/lexer.yy.cc $<

# Compile the parser
$(OUTPUT_PATH)/parser.tab.o: $(OUTPUT_PATH)/parser.tab.cc
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) $(LDFLAGS) -c $(OUTPUT_PATH)/parser.tab.cc -o $(OUTPUT_PATH)/parser.tab.o

# Generate the parser
# Expects bison 3.7 (brew install bison)
$(OUTPUT_PATH)/parser.tab.cc $(OUTPUT_PATH)/parser.tab.hpp $(OUTPUT_PATH)/location.hh $(OUTPUT_PATH)/FlexLexer.h: parser.yy scanner.hpp
	mkdir -p $(OUTPUT_PATH)
	bison $(BISON_FLAGS) $< --output=$(OUTPUT_PATH)/parser.tab.cc --defines=$(OUTPUT_PATH)/parser.tab.hpp
ifeq ($(shell uname),Darwin)
	cp /usr/local/opt/flex/include/FlexLexer.h $(OUTPUT_PATH)
else
	cp /usr/include/FlexLexer.h $(OUTPUT_PATH)
endif

clean:
	rm -r build &> /dev/null || true
