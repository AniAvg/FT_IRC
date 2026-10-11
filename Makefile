NAME = ircserv

SRCS = main.cpp \
		Server.cpp \
		Client.cpp \
		Channel.cpp \
		CommandHandler.cpp

SRC_DIR = srcs
OBJ_DIR = objs

OBJS = $(addprefix $(OBJ_DIR)/,$(SRCS:.cpp=.o))


CXX = c++
CXXFLAGS = -Wall -Wextra -Werror -std=c++98


all: $(NAME)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -Iincludes -c $< -o $@

$(NAME): $(OBJS)
	$(CXX) $(OBJS) $(CXXFLAGS) -o $(NAME)

clean:
	rm -rf $(OBJ_DIR)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re