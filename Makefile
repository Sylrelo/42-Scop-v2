NAME	= scop
SRCS 	= main.c parsing_init.c parsing_mtl.c parsing_texture.c gl3w.c init_shader.c parse_faces.c print_debug.c vec_utils.c

OBJ_DIR = ./.Objects
SRC_DIR = ./Sources
INC_DIR = ./Includes 
INC_HDR =  -I${INC_DIR} 
INC_LIB = -framework OpenGL -lglfw


CFLAGS	:= -Wall -Wextra -O3 -I$(INC_DIR)
OBJS	:= $(patsubst %.c,$(OBJ_DIR)/%.o,  $(SRCS))

all: CREATE_OBJDIR $(NAME)

CREATE_OBJDIR:
		#@clear
		@mkdir -p $(OBJ_DIR)

$(NAME): $(OBJS)
		@gcc ${CFLAGS} ${INC_LIB} $^ -o ${NAME}
		@printf "\033[1m[ SCOP ]\033[1m\t\tcompilation complete.\t\033[0m\n"

$(OBJ_DIR)/%.o : $(SRC_DIR)/%.c 
		@gcc $(CFLAGS) $(INC_HDR) -o $@ -c $<

mk: 
	@mkdir -p .Objects

clean:
		@rm -rf $(OBJ_DIR)
		@echo "\033[1m[ SCOP ]\033[0m\t\tobjects cleared.\033[0m"

fclean : clean
		@rm -f $(NAME)
		@echo "\033[1m[ SCOP ]\033[0m\t\tbinary deleted.\033[0m"

re:		fclean all
