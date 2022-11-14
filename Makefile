#######################################
#              COMPILER               #
#######################################

CC	=	clang++

######################################
#             EXECUTABLE             #
######################################
NAME	=	ircserv

#######################################
#             DIRECTORIES             #
#######################################

OBJSDIR	=	objs

######################################
#            SOURCE FILES            #
######################################

SRCS_DIR	=	srcs/

SRCS	=	main.cpp		\
			Server.cpp		\
			Client.cpp		\
			utils.cpp		\
			parser.cpp		\
			Channel.cpp		\
			Commands.cpp	\
			execution_utils.cpp

INC		= includes
######################################
#            OBJECT FILES            #
######################################

OBJS	=	${addprefix ${OBJSDIR}/, ${SRCS:.cpp=.o}}

DEP	=	${OBJS:.o=.d}

#######################################
#                FLAGS                #
#######################################

FLAGS = -Wall -Werror -Wextra -MMD -MP -std=c++98 -I $(INC) -g3 -Wno-unused-parameter

#######################################
#                RULES                #
#######################################

.PHONY: clean fclean all re

all: ${NAME}

$(NAME): ${OBJS}
			${CC} ${FLAGS} ${OBJS} -o $@

-include $(DEP)
${OBJSDIR}/%.o:${SRCS_DIR}%.cpp | $(OBJSDIR)
			${CC} ${FLAGS} -c $< -o $@

$(OBJSDIR):
	@mkdir -p $@

clean:
		rm -rf ${OBJSDIR}

fclean: clean
		rm -rf ${NAME}

re:	fclean all
