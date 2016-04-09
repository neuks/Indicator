############################################################################### 
# Visual K-Line Analysing System For Zen Theory
# Copyright (C) 2016, Martin Tang
#
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <http://www.gnu.org/licenses/>.
############################################################################### 

# Configurations
INCLUDE=
ASFLAGS=$(INCLUDE) -O2
CCFLAGS=$(INCLUDE) -O2
CXFLAGS=$(INCLUDE) -O2
FCFLAGS=$(INCLUDE) -O2
LDFLAGS=

# Objectives
OBJECT1=Main.o CCentroid.o
TARGET1=CZSC.dll
OBJECTS=$(OBJECT1)
TARGETS=$(TARGET1)
DEPENDS=$(OBJECTS:.o=.dep) 

# Build Commands
all : $(TARGETS)

$(TARGET1) : $(OBJECTS) 
	@echo [LD] $@
	@c++ -shared -o $@ $^ $(LDFLAGS)

debug: all
	@echo [DB] $(TARGETS)
	@gdb -w $(TARGETS)

run: all 
	@echo [EX] $(TARGETS)
	@$(TARGETS)

clean:  
	@echo [RM] $(OBJECTS)
	@rm $(DEPENDS) $(OBJECTS)

# Standard Procedures
%.dep : %.s 
	@gcc $(INCLUDE) -MM -MT $(@:.dep=.o) -o $@ $< 

%.dep : %.c 
	@gcc $(INCLUDE) -MM -MT $(@:.dep=.o) -o $@ $< 

%.dep : %.m 
	@gcc $(INCLUDE) -MM -MT $(@:.dep=.o) -o $@ $< 

%.dep : %.cpp 
	@gcc $(INCLUDE) -MM -MT $(@:.dep=.o) -o $@ $< 

%.dep : %.f 
	@gcc $(INCLUDE) -MM -MT $(@:.dep=.o) -o $@ $< 

%.dep : %.rc 
	@gcc $(INCLUDE) -MM -MT $(@:.dep=.o) -o $@ $< 

%.dep : %.l
	@gcc $(INCLUDE) -MM -MT $(@:.dep=.o) -o $@ $< 

%.dep : %.y
	@gcc $(INCLUDE) -MM -MT $(@:.dep=.o) -o $@ $< 

%.o : %.s 
	@echo [AS] $<
	@as $(ASFLAGS) -o $@ $< 

%.o : %.c 
	@echo [CC] $<
	@gcc $(CCFLAGS) -c -o $@ $< 

%.o : %.m 
	@echo [OC] $<
	@gcc $(CCFLAGS) -c -o $@ $< 

%.o : %.cpp 
	@echo [CX] $<
	@c++ $(CXFLAGS) -c -o $@ $< 

%.o : %.f
	@echo [CX] $<
	@g77 $(FCFLAGS) -c -o $@ $< 

%.o : %.rc
	@echo [CX] $<
	@windres $< $@ 

%.c : %.l
	@echo [FL] $<
	@flex -o $@ $<

%.c : %.y
	@echo [BS] $<
	@bison -d -o $@ $<

-include $(DEPENDS)
