#include "main.hpp"


using namespace DBL;

const char *MessageQueue::message_queue_file = "message queue.bin";

Color ColorStringPair::common;
void ColorStringPair::SetCommonColor(Color color)
{
	common = color;
}

ColorStringPair ColorStringPair::Split(size_t pos)
{
	std::string nextStr = str.substr(pos);
	str = str.substr(0, pos);
			
	ColorStringPair ret(color, nextStr);

	return ret;
}

MessageQueue::MessageQueue():
MESSAGE_QUEUE_WIDTH_IN_PIXELS(0),
MESSAGE_QUEUE_HEIGHT_IN_LINES(6)
{}
MessageQueue::~MessageQueue(){}

void MessageQueue::Initialize()
{
	MESSAGE_QUEUE_WIDTH_IN_PIXELS = SCREEN_WIDTH()-100;
	LoadFromFile();
}

void MessageQueue::Save()
{
	SaveToFile();
}

//TODO
void MessageQueue::ClearQueueFile(){}
void MessageQueue::LoadFromFile()
{
	//Open file
	const int BUF_SIZE = 512;
	char buffer[BUF_SIZE];
	ALLEGRO_FILE *file = al_fopen(message_queue_file, "r");
	if(file)
	{
		int line = 0;
		Color setColor;
		ColorfulString newline;
		while(al_fgets(file, buffer, BUF_SIZE))
		{
			line++;
			std::string str = buffer;

			//remove the \n
			if(!str.empty() && str[str.length()-1] == '\n') str.erase(str.length()-1);
			if(str[0] == 'C')
			{
				str = str.substr(1); //remove the first character
				auto tokenized = split(str, ':');
				setColor.r = atoi(tokenized[0].c_str());
				setColor.g = atoi(tokenized[1].c_str());
				setColor.b = atoi(tokenized[2].c_str());
				setColor.a = atoi(tokenized[3].c_str());
			}
			else if(str[0] == 'T')
			{
				str = str.substr(1);
				newline.Add(ColorStringPair(setColor, str));

			}
			else if(str[0] == 'N')
			{
				AddMessage(newline);
				newline.line.clear();
			}
			//else it's not a valid line

		}
		al_fclose(file);
	}
}

void MessageQueue::SaveToFile()
{
	ALLEGRO_FILE *file = al_fopen(message_queue_file, "w");
	if(file)
	{
		bool isFirstColor = true;
		Color previousColor;
		for(auto line = lines.begin(); line != lines.end(); line++)
		{
			for(auto segment = (*line).line.begin(); segment != (*line).line.end(); segment++)
			{
				if(segment->color != previousColor || isFirstColor)
				{
					isFirstColor = false;
					//color changed
					previousColor = segment->color;
					std::ostringstream colorStream;
					colorStream << "C" << (int)previousColor.r << ":" <<
									 (int)previousColor.g << ":" <<
									 (int)previousColor.b << ":" <<
									 (int)previousColor.a << "\n";
				
					al_fwrite(file, colorStream.str().c_str(), strlen(colorStream.str().c_str()));
					
				}

				//Add line
				std::ostringstream stream;
				stream << "T" <<  segment->str.c_str() << "\n";
				al_fwrite(file, stream.str().c_str(), strlen(stream.str().c_str()));
			}
			std::stringstream  stream;
			stream << "N\n";
			al_fwrite(file, stream.str().c_str(), strlen(stream.str().c_str()));

			//Make newline
		}

		al_fclose(file);
	}
}

void MessageQueue::AddMessage(ColorfulString addedLine)
{
	do
	{
		auto str = addedLine.Str();
		int lineWidth = game->graphics.GetTextWidth(COMMON16, str);
		if(lineWidth < MESSAGE_QUEUE_WIDTH_IN_PIXELS) //whole line fits
		{
			addedLine.TrimLeadingWhiteSpace();
			lines.push_front(addedLine);
			addedLine.line.clear();
		}
		else //split it in a good spot
		{
			size_t LineSplitPosition = 0;
			auto words = split(str, ' ');
			if(words.size() == 1) //there are no good spots (no spaces in a very long line)
			{
				/* We were sent a line that has no spaces,
				someone is doing something dumb (or reusing this code for a language without spaces). 
				Let's just get through this. We don't have to do this often, just take the slowdown */
				const int ARBITRARY_INCREMENT = 1;
				do
				{
					LineSplitPosition += ARBITRARY_INCREMENT;

				} while(game->graphics.GetTextWidth(COMMON16, str.substr(0, LineSplitPosition)) < MESSAGE_QUEUE_WIDTH_IN_PIXELS);
				LineSplitPosition -= ARBITRARY_INCREMENT;
			
			}
			else
			{
				//find as much as we can fit
				int numberOfWordsThatFit = 0;
				int accumulatedPixelWidth = 0;
				while(numberOfWordsThatFit < (int)words.size())
				{
					int wordLengthInPixels = game->graphics.GetTextWidth(COMMON16, words[numberOfWordsThatFit] + " ");
					if(accumulatedPixelWidth + wordLengthInPixels < MESSAGE_QUEUE_WIDTH_IN_PIXELS)
					{
						LineSplitPosition += 1 + words[numberOfWordsThatFit].length(); //1=removed space
						numberOfWordsThatFit++;
						accumulatedPixelWidth += wordLengthInPixels;
					}
					else break; //no more words fit
				}
			
			}
			
			//Split at the spot we found
			auto EndOfLine = addedLine.Split(LineSplitPosition);
			addedLine.TrimLeadingWhiteSpace();
			lines.push_front(addedLine);
			addedLine = EndOfLine;
		}

	} while(!addedLine.line.empty());
}

void MessageQueue::Draw(int lines, bool hasBackground)
{
	if(lines == 0) lines = MESSAGE_QUEUE_HEIGHT_IN_LINES;

	game->graphics.SetPrintAlignment(PRINT_LEFT);
	
	int y = SCREEN_HEIGHT() - game->graphics.GetTextLineHeight(COMMON16);
	game->graphics.DrawColoredBox(25, SCREEN_HEIGHT() - game->graphics.GetTextLineHeight(COMMON16)*lines,
		game->world.messageQueue.MESSAGE_QUEUE_WIDTH_IN_PIXELS+25, SCREEN_HEIGHT(), Color(0, 0, 0, 64));
	for(auto msg = game->world.messageQueue.lines.begin(); msg !=
		game->world.messageQueue.lines.end(); msg++)
	{
		if(lines > 0)
		{
			int x = 0;
			for(auto segment = (*msg).line.begin(); segment != (*msg).line.end(); segment++)
			{
				game->graphics.PrintText(COMMON16, (*segment).color, 25+x, y, (*segment).str);
				x += game->graphics.GetTextWidth(COMMON16, (*segment).str);
			}
				
			y -= game->graphics.GetTextLineHeight(COMMON16);
			lines--;
		} else break;
			
	}
}