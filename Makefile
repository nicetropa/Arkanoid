TOPDIR  := ./

SRCDIR_CLIENT  := $(TOPDIR)src/
OBJDIR_CLIENT  := $(TOPDIR)obj/
BINDIR_CLIENT  := $(TOPDIR)
NAME_CLIENT    := Arkanoid
EXE_CLIENT     := $(BINDIR)$(NAME_CLIENT)

SFILES  := cpp
OFILES  := o
CC      := g++
CFLAGS  := -Wall -Wextra -O2 -std=gnu++17 -Wpedantic -march=native -Wnull-dereference -Winline -fsanitize=address,undefined -lallegro -lallegro_font -lallegro_ttf -lallegro_primitives -lallegro_image
LIBS    := -fsanitize=address,undefined -lallegro -lallegro_font -lallegro_ttf -lallegro_primitives -lallegro_image

SOURCES_CLIENT := $(shell find $(SRCDIR_CLIENT) -name "*.$(SFILES)")
OBJECTS_CLIENT := $(patsubst $(SRCDIR_CLIENT)%.$(SFILES), $(OBJDIR_CLIENT)%.$(OFILES), $(SOURCES_CLIENT))

ALLFILES := $(SOURCES_CLIENT)

.PHONY: all clean

all: $(EXE_CLIENT) $(EXE_SERVEUR)

$(EXE_CLIENT): $(OBJECTS_CLIENT)
	$(CC) $^ -o $@ $(LIBS)

$(OBJDIR_CLIENT):
	@mkdir -p $(OBJDIR_CLIENT)

$(OBJDIR_CLIENT)%$(OFILES): $(SRCDIR_CLIENT)%$(SFILES) | $(OBJDIR_CLIENT)
	$(CC) $(CFLAGS) $< -c -o $@
	
clean:
	@rm -rf $(OBJDIR_CLIENT)
	@rm -f $(EXE_CLIENT)
