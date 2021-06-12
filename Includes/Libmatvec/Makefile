NAME	= libmatvec.a

SRCS 	=	vecs.c matrice.c mat_projection.c mat_supl.c mat_transform.c mat_vec3f.c

OBJ_DIR = ./.Objects
SRC_DIR = ./Sources
INC_DIR = ./Includes

OBJS	:= $(patsubst %.c, $(OBJ_DIR)/%.o, $(SRCS))

CFLAGS	:= -Wall -Wextra -Werror -O3 -I$(INC_DIR)

print_lib =  \033[1m[\033[1m\033[33m LIBMATVEC \033[0m\033[1m]\033[0m  

all: CREATE_OBJDIR $(NAME)

CREATE_OBJDIR:
		@mkdir -p $(OBJ_DIR)

$(NAME): $(OBJS)
		@ar -rc $(NAME) $(OBJS)
		@ranlib $(NAME)
		@printf "$(print_lib)\033[1m library successfully created.\033[0m\n"

$(OBJ_DIR)/%.o : $(SRC_DIR)/%.c $(INC_DIR)/Libmatvec.h
		@gcc $(CFLAGS) -o $@ -c $<

clean:
		@rm -rf ./.Objects/$(OBJS)
		@printf "$(print_lib) objects cleared.\n"

fclean : clean
		@rm -f $(NAME)
		@printf "$(print_lib) binary deleted.\n"

re:		fclean all