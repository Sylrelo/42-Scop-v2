NAME	= scop
SRCS 	= main.c parsing_init.c parsing_mtl.c parsing_texture.c gl3w.c parse_shader.c parse_faces.c scop_utils.c glfw_events.c glx_init.c display.c shadowmap.c

OBJ_DIR = ./.Objects
SRC_DIR = ./Sources
INC_DIR = ./Includes 
INC_HDR =  -I${INC_DIR} -I./Includes/Libmatvec/Includes 
LIBS_DIR = -L./Includes/Libmatvec

INC_LIB := ${LIBS_DIR} -lmatvec 

ifeq ($(OS),Windows_NT)
INC_LIB += -lopengl32 -lglfw3 -lglfw3dll -lopengl32 -lgdi32 -Wl,-u,___mingw_vsnprintf -Wl,--defsym,___ms_vsnprintf=___mingw_vsnprintf
else
INC_LIB += -framework OpenGL -lglfw
endif

CFLAGS	:= -Wall -Wextra -Werror -O3 -I$(INC_DIR)
OBJS	:= $(patsubst %.c,$(OBJ_DIR)/%.o, $(SRCS))

all: CREATE_OBJDIR COMPILE_EXTERN_LIBS $(NAME)

COMPILE_EXTERN_LIBS:
		@make -C ./Includes/Libmatvec
		${project_path}
CREATE_OBJDIR:
		@mkdir -p $(OBJ_DIR)

$(NAME): $(OBJS)
		@gcc ${CFLAGS} $^ ${INC_LIB} -o ${NAME} 
		@printf "\033[1mScop \033[1m : compiled.\t\033[0m\n"

$(OBJ_DIR)/%.o : $(SRC_DIR)/%.c Includes/Libmatvec/libmatvec.a Includes/Scop.h
		@gcc $(CFLAGS) $(INC_HDR) -o $@ -c $<

mk: 
	@mkdir -p .Objects

run:
	@make
	./scop Resources/Wavefront/42.obj

clean:
		@rm -rf $(OBJ_DIR)
		@make clean -C ./Includes/Libmatvec
		@echo "\033[1mScop\033[0m : objects cleared.\033[0m"

fclean : clean
		@rm -f $(NAME)
		@make fclean -C ./Includes/Libmatvec
		@echo "\033[1mScop\033[0m : binary deleted.\033[0m"

re:		fclean all
