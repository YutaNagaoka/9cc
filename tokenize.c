#include "ycc.h"

Map *keywords;

Token *add_token(Vector *v, int type, char *input)
{
	Token *t = malloc(sizeof(Token));
	t->type = type;
	t->input = input;
	vec_push(v, t);
	return t;
}

bool is_alnum(char c)
{
	return ('a' <= c && c <= 'z') ||
		   ('A' <= c && c <= 'Z') ||
		   ('0' <= c && c <= '9') ||
		   (c == '_');
}

Vector *tokenize(char *pos)
{
	keywords = new_map();
	map_set(keywords, "if", (void *)TK_IF);
	map_set(keywords, "return", (void *)TK_RETURN);
	return scan(pos);
}

Vector *scan(char *pos)
{
	Vector *v = new_vector();
	while (*pos)
	{
		if (isspace(*pos))
		{
			pos++;
		}
		else if (isdigit(*pos))
		{
			Token *t = add_token(v, TK_NUM, pos);
			t->value = strtol(pos, &pos, 10);
		}
		else if (!strncmp(pos, "==", 2))
		{
			add_token(v, TK_EQ, pos);
			pos += 2;
		}
		else if (!strncmp(pos, "!=", 2))
		{
			add_token(v, TK_NE, pos);
			pos += 2;
		}
		else if (!strncmp(pos, "<=", 2))
		{
			add_token(v, TK_LE, pos);
			pos += 2;
		}
		else if (!strncmp(pos, ">=", 2))
		{
			add_token(v, TK_GE, pos);
			pos += 2;
		}
		else if (!strncmp(pos, "return", 6) && !is_alnum(pos[6]))
		{
			add_token(v, TK_RETURN, pos);
			pos += 6;
		}
		else if (isalpha(*pos))
		{
			int length = 1;
			while (isalnum(pos[length]))
				length++;

			char *name = strndup(pos, length);
			int type = (intptr_t)map_get(keywords, name);
			if (!type)
			    type = TK_IDENT;
			Token *t = add_token(v, type, pos);
			t->name = name;
			pos += length;
		}
		else if (*pos == '+' || *pos == '-' || *pos == '*' || *pos == '/' || *pos == '(' ||
				 *pos == ')' || *pos == '<' || *pos == '>' || *pos == '=' || *pos == ';')
		{
			add_token(v, *pos, pos);
			pos++;
		}
		else
		{
			fprintf(stderr, "Cannot tokenize: %s\n", pos);
			exit(1);
		}
	}
	add_token(v, TK_EOF, pos);
	return v;
}