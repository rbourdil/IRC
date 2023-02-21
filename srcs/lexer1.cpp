#include "lexer.hpp"

bool	is_special_char(int c)
{
	if (c == '-' || c == ']' || c == '[' || c == '\\' || c == '`' \
		|| c == '^' || c == '{' || c == '}')
		return (true);
	return (false);
}

token	lexer::get_token(void)
{
	int	type, c;

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
				{
					set_type(END_TOKEN);
					set_state(ACCEPT_STATE);
				}
				else if (c == ' ')
				{
					set_type(SPACE_TOKEN);
					set_state(SPACE_STATE);
				}
				else if (isalpha(c))
				{
					set_type(COMMAND_L_TOKEN);
					set_state(COMMAND_L_STATE);
				}
				else if (isdigit(c))
				{
					set_type(COMMAND_D_TOKEN);
					set_state(COMMAND_D_STATE);
				}
				else
				{
					set_type(MIDDLE_TOKEN);
					set_state(MIDDLE_STATE);
				}
				break;
			case SPACE_STATE:
				if (c == '\0')
					return (token(END_TOKEN));
				else if (c != ' ')
				{
					_scan.ungetc();
					state = ACCEPT_STATE;
				}
				break;
			case COMMAND_L_STATE:
				if (c == '\0')
				{
					_scan.ungetc();
					return (token(END_TOKEN));
				}
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
					set_type(MIDDLE_TOKEN);
					set_state(MIDDLE_STATE);
				}
				break;
			case COMMAND_D_STATE:
				if (c == '\0')
				{
					_scan.ungetc();
					return (token(END_TOKEN));
				}
				else if (c == ' ' || c == CR || c == LF)
				{
					_scan.ungetc();
					set_type(MIDDLE_TOKEN);
					set_state(ACCEPT_STATE);
				}
				else if (!isdigit(c))
				{
					set_type(MIDDLE_TOKEN);
					set_state(MIDDLE_STATE);
				}
				else
					set_state(COMMAND_D_STATE1);	
				break;
			case COMMAND_D_STATE1:
				if (c == '\0')
				{
					_scan.ungetc();
					return (token(END_TOKEN));
				}
				else if (c == ' ' || c == CR || c == LF)
				{
					_scan.ungetc();
					set_type(MIDDLE_TOKEN);
					set_state(ACCEPT_STATE);
				}
				else if (!isdigit(c))
				{
					set_type(MIDDLE_TOKEN);
					set_state(MIDDLE_STATE);
				}
				else
					set_state(COMMAND_D_STATE2);
				break;
			case COMMAND_D_STATE2:
				if (c == '\0')
				{
					_scan.ungetc();
					return (token(END_TOKEN));
				}
				else if (c == ' ' || c == CR || c == LF)
				{
					_scan.ungetc();
					set_type(CMD_D_TOKEN);
					set_state(ACCEPT_STATE);
				}
				else
				{
					set_type(MIDDLE_TOKEN);
					set_state(MIDDLE_STATE);
				}


}
