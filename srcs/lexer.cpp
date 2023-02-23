#include "lexer.hpp"

inline bool	is_special_char(int c)
{
	if (c == '-' || c == ']' || c == '[' || c == '\\' || c == '`' \
		|| c == '^' || c == '{' || c == '}')
		return (true);
	return (false);
}

token	lexer::get_token(void)
{
	std::string	lexeme;
	int			c;

	c = _scan.getc();
	while (get_state() != ACCEPT_STATE)
	{
		switch (get_state())
		{
			case START_STATE:
				if (c == ':')
				{
					set_type(COLON_TOKEN);
					set_state(ACCEPT_STATE);
				}
				else if (c == '!')
				{
					set_type(EXCL_TOKEN);
					set_state(ACCEPT_STATE);
				}
				else if (c == '@')
				{
					set_type(AT_TOKEN);
					set_state(ACCEPT_STATE);
				}
				else if (c == CR)
				{
					set_type(CR_TOKEN);
					set_state(ACCEPT_STATE);
				}
				else if (c == LF)
				{
					set_type(LF_TOKEN);
					set_state(ACCEPT_STATE);
				}
				else if (c == '\0')
					return (token(END_TOKEN));
				else if (c == ' ')
				{
					set_type(SPACE_TOKEN);
					set_state(SPACE_STATE);
				}
				else if (isalpha(c))
				{
					set_type(CMD_L_TOKEN);
					set_state(CMD_L_STATE);
				}
				else if (isdigit(c))
				{
					set_type(CMD_D_TOKEN);
					set_state(CMD_D_STATE);
				}
				else
				{
					set_type(PARAM_TOKEN);
					set_state(PARAM_STATE);
				}
				break;
			case SPACE_STATE:
				if (c == '\0')
					return (token(END_TOKEN));
				else if (c != ' ')
				{
					_scan.ungetc();
					set_state(ACCEPT_STATE);
				}
				break;
			case CMD_L_STATE:
				if (c == '\0')
					return (token(END_TOKEN));
				else if (isdigit(c) || c == '-')
				{
					set_type(NAME_TOKEN);
					set_state(NAME_STATE);
				}
				else if (c == '.')
				{
					set_type(SRV_NAME_TOKEN);
					set_state(SRV_NAME_STATE);
				}
				else if (is_special_char(c))
				{
					set_type(NICK_TOKEN);
					set_state(NICK_STATE);
				}
				else if (c == ' ' || c == CR || c == LF)
				{
					_scan.ungetc();
					set_state(ACCEPT_STATE);
				}
				else if (!isalpha(c))
				{
					set_type(PARAM_TOKEN);
					set_state(PARAM_STATE);
				}
				break;
			case CMD_D_STATE:
				if (c == '\0')
					return (token(END_TOKEN));
				else if (c == ' ' || c == CR || c == LF)
				{
					_scan.ungetc();
					set_type(PARAM_TOKEN);
					set_state(ACCEPT_STATE);
				}
				else if (!isdigit(c))
				{
					set_type(PARAM_TOKEN);
					set_state(PARAM_STATE);
				}
				else
					set_state(CMD_D_STATE1);	
				break;
			case CMD_D_STATE1:
				if (c == '\0')
				{
					_scan.ungetc();
					return (token(END_TOKEN));
				}
				else if (c == ' ' || c == CR || c == LF)
				{
					_scan.ungetc();
					set_type(PARAM_TOKEN);
					set_state(ACCEPT_STATE);
				}
				else if (!isdigit(c))
				{
					set_type(PARAM_TOKEN);
					set_state(PARAM_STATE);
				}
				else
					set_state(CMD_D_STATE2);
				break;
			case CMD_D_STATE2:
				if (c == '\0')
				{
					_scan.ungetc();
					return (token(END_TOKEN));
				}
				else if (c == ' ' || c == CR || c == LF)
				{
					_scan.ungetc();
					set_state(ACCEPT_STATE);
				}
				else
				{
					set_type(PARAM_TOKEN);
					set_state(PARAM_STATE);
				}
				break;
			case PARAM_STATE:
				if (c == '\0')
				{
					_scan.ungetc();
					return (token(END_TOKEN));
				}
				else if (c == ' ' || c == CR || c == LF)
				{
					_scan.ungetc();
					set_state(ACCEPT_STATE);
				}
				break;
			case NAME_STATE:
				if (c == '\0')
				{
					_scan.ungetc();
					return (token(END_TOKEN));
				}
				else if (c == ' ' || c == CR || c == LF)
				{
					_scan.ungetc();
					set_state(ACCEPT_STATE);
				}
				else if (c == '.')
					set_state(POINT_STATE);
				else if (c != '-' && is_special_char(c))
				{
					set_type(NICK_TOKEN);
					set_state(NICK_STATE);
				}
				else if (!isalpha(c) && !isdigit(c))
				{
					set_type(PARAM_TOKEN);
					set_state(PARAM_STATE);
				}
				break;
			case SRV_NAME_STATE:
				if (c == '\0')
				{
					_scan.ungetc();
					return (token(END_TOKEN));
				}
				else if (c == ' ' || c == CR || c == LF)
				{
					_scan.ungetc();
					set_state(ACCEPT_STATE);
				}
				else if (c == '.')
					set_state(POINT_STATE);
				else if (c != '-' && is_special_char(c))
				{
					set_type(NICK_TOKEN);
					set_state(NICK_STATE);
				}
				else if (!isalpha(c) && !isdigit(c))
				{
					set_type(PARAM_TOKEN);
					set_state(PARAM_STATE);
				}
				break;
			case POINT_STATE:
				if (c == '\0')
				{
					_scan.ungetc();
					return (token(END_TOKEN));
				}
				else if (!isalpha(c))
				{
					_scan.ungetc();
					set_type(ERROR_TOKEN);
					set_state(ACCEPT_STATE);
				}
				else
					set_state(SRV_NAME_STATE);
				break;
			case NICK_STATE:
				if (c == '\0')
				{
					_scan.ungetc();
					return (token(END_TOKEN));
				}
				else if (c == ' ' || c == CR || c == LF)
				{
					_scan.ungetc();
					set_state(ACCEPT_STATE);
				}
				else if (!isalpha(c) && !isdigit(c) && !is_special_char(c))
				{
					set_type(PARAM_TOKEN);
					set_state(PARAM_STATE);
				}
				break;
			default: // should never happen
				std::cerr << "unrecognized state" << std::endl;
				std::cerr << get_state() << std::endl;
				exit(EXIT_FAILURE);
		}
		c = _scan.getc();
	}
	_scan.ungetc();
	set_state(START_STATE);
	lexeme = _scan.flush();
	return (token(get_type(), lexeme));
}
