# Definitions de macros
OUT = projet
CXX = g++
CXXFLAGS = -Wall -std=c++17
LINKING = `pkg-config --cflags gtkmm-4.0`
LDLIBS = `pkg-config --libs gtkmm-4.0`
CXXFILES = projet.cc simulation.cc robot.cc particule.cc message.cc shape.cc gui.cc graphic.cc
OFILES = projet.o simulation.o robot.o particule.o message.o shape.o gui.o graphic.o

# Definition de la premiere regle

all: $(OUT)

projet.o: projet.cc
		$(CXX) $(CXXFLAGS) $(LINKING) -c $< -o $@ $(LDLIBS)
        
simulation.o: simulation.cc simulation.h
		$(CXX) $(CXXFLAGS) $(LINKING) -c $< -o $@ $(LDLIBS)

robot.o: robot.cc robot.h
		$(CXX) $(CXXFLAGS) $(LINKING) -c $< -o $@ $(LDLIBS)
        
particule.o: particule.cc particule.h
		$(CXX) $(CXXFLAGS) $(LINKING) -c $< -o $@ $(LDLIBS)

message.o: message.cc message.h
		$(CXX) $(CXXFLAGS) $(LINKING) -c $< -o $@ $(LDLIBS)

shape.o: shape.cc shape.h
		$(CXX) $(CXXFLAGS) $(LINKING) -c $< -o $@ $(LDLIBS)

gui.o: gui.cc gui.h
		$(CXX) $(CXXFLAGS) $(LINKING) -c $< -o $@ $(LDLIBS)
               
graphic.o: graphic.cc graphic.h
		$(CXX) $(CXXFLAGS) $(LINKING) -c $< -o $@ $(LDLIBS)

$(OUT): $(OFILES)
		$(CXX) $(CXXFLAGS) $(LINKING) $(OFILES) -o $@ $(LDLIBS)

# Definitions de cibles particulieres

depend:
	@echo " *** MISE A JOUR DES DEPENDANCES ***"
	@(sed '/^# -- Regles de dependances generees automatiquement/q' Makefile && \
	  $(CXX) -MM $(CXXFLAGS) $(CXXFILES) | \
	  egrep -v "(/usr/include)" \
	 ) >Makefile.new
	@mv Makefile.new Makefile

clean:
	@echo " *** EFFACE MODULES OBJET ET EXECUTABLE ***"
	@/bin/rm -f *.o *.x *.cc~ *.h~ $(OUT)

#
# -- Regles de dependances generees automatiquement
projet.o: projet.cc simulation.h robot.h particule.h shape.h gui.h \
 graphic.h
simulation.o: simulation.cc constantes.h shape.h simulation.h robot.h \
 particule.h message.h
robot.o: robot.cc shape.h message.h robot.h particule.h constantes.h
particule.o: particule.cc shape.h message.h particule.h constantes.h
message.o: message.cc message.h
shape.o: shape.cc shape.h graphic.h
gui.o: gui.cc gui.h simulation.h robot.h particule.h shape.h graphic.h
graphic.o: graphic.cc graphic.h
