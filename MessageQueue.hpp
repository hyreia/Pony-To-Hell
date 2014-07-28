#ifndef MESSAGE_QUEUE_HPP
#define MESSAGE_QUEUE_HPP

/* Message queue file format:
Each line begins with either a T, C or or N to signify what the rest of the line is
C means it's telling you what the color of the next lines are
T means it's giving another part of the current line
N means it's starting a new line

So where the color doesn't change much, it keeps adding lines of the same color as
previously set.

A possible (abstract) example would be:
C white
T "This is part of a line that is white and "
C yellow
T "this part is yellow"
N
C white
T "new line that is white"

*/
namespace DBL
{
	static const char *message_queue_file;

	struct ColorStringPair
	{
		static void SetCommonColor(Color color);
		static Color common;
		Color color;
		std::string str;
		ColorStringPair(const std::string &_str):
		color(common),
		str(_str)
		{}
		ColorStringPair(Color _color, const std::string &_str):
		color(_color),
		str(_str)
		{}

		ColorStringPair Split(size_t pos);
	};

	struct ColorfulString
	{
		ColorfulString(){}
		ColorfulString(const std::string &str)
		{
			line.push_back(ColorStringPair(str));
		}
		ColorfulString(Color color, const std::string &str)
		{
			line.push_back(ColorStringPair(color, str));
		}

		std::list<ColorStringPair> line;

		inline void Add(ColorStringPair segment)
		{
			line.push_back(segment);
		}

		inline size_t Length()
		{
			size_t length = 1; //1 = '\0'
			for(auto segment = line.begin(); segment != line.end(); segment++)
			{
				length += (*segment).str.length();
			}
			return length;
		}
		
		inline std::string Str()
		{
			std::string ret;
			ret.reserve(Length());
			for(auto segment = line.begin(); segment != line.end(); segment++)
			{
				ret.append( (*segment).str );
			}

			return ret;
		}

		inline ColorfulString Split(size_t pos)
		{
			ColorfulString ret;

			size_t lengthLeft = pos;
			for(auto segment = line.begin(); segment != line.end(); segment++)
			{
				if( (*segment).str.length() < lengthLeft)
				{
					lengthLeft -= (*segment).str.length();
				}
				else
				{
					
					auto firstSegment = (*segment).Split(lengthLeft);
					ret.Add(firstSegment);
					segment++;
					while(segment != line.end())
					{
						ret.Add( (*segment) );
						segment = line.erase(segment);
					}
					break;
				}
			}

			return ret;
		}

		inline void TrimLeadingWhiteSpace()
		{
			if(!line.empty())
			{
				size_t pos = line.front().str.find_first_not_of(' ');
				if(pos != std::string::npos)
					line.front().str = line.front().str.substr(pos);
			}
		}
	};

	class MessageQueue
	{
	public:
		MessageQueue();
		~MessageQueue();

		/* Initialize display first */
		void Initialize();
		void Save();
		void ClearQueueFile();
		void Draw(int lines = 0, bool hasBackground = true);



		/* Always add with this, it splits up lines too long */
		void AddMessage(ColorfulString addedLine);

		int MESSAGE_QUEUE_WIDTH_IN_PIXELS;
		const int MESSAGE_QUEUE_HEIGHT_IN_LINES;
		std::list<ColorfulString> lines;

	private:
		void LoadFromFile();
		void SaveToFile();

		static const char *message_queue_file;
	};

};

#endif