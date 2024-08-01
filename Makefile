NAME = game
CPPFLAGS = -Wall -Weffc++
INCLUDES = -I./include/
LIBS = -lglut -lGLU -lGL
SRCS = ./src/main.cpp \
		./src/display.cpp \
		./src/keyboard.cpp \
		./src/reshape.cpp \
		./src/timer.cpp \
		./src/lightInit.cpp \
		./src/map.cpp
OBJS = $(SRCS:.cpp=.o)

all : $(NAME)

run : $(NAME)
	./$(NAME)

$(NAME) : $(OBJS)
	g++ $(CPPFLAGS) $(INCLUDES) $(OBJS) $(LIBS) -o $(NAME)

%.o : %.cpp
	g++ $(CPPFLAGS) $(INCLUDES) -c $< $(LIBS) -o $@

clean :
	$(RM) $(OBJS)

fclean : clean
	$(RM) $(NAME)

re : fclean all

.PHONY : all clean fclean re
