NAME = game
CPPFLAGS = -Wall -Weffc++
INCLUDES = -L ./include
LIBS = -lglut -lGLU -lGL
SRCS = ./src/main.cpp
OBJS = $(SRCS:.cpp=.o)

all : $(NAME)

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
