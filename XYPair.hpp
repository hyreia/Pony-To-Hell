#ifndef XYPAIR_HPP
#define XYPAIR_HPP

/* Super simple struct containing two elements typically used for coordinates
or dimensions (hence X and Y) and a small handful of operations with them. */

template <typename T = int> struct XYPair
{
	T x, y;
	XYPair(): x(0), y(0)
	{}

	XYPair(T _x, T _y): x(_x), y(_y)
	{}

	bool operator==(XYPair<T> &other)
	{
		return(x == other.x && y == other.y);
	}

	bool operator!=(XYPair<T> &other)
	{
		return !operator==(other);
	}

	T distanceSquared(const XYPair<T> &other)
	{
		T dx = x - other.x;
		T dy = y - other.y;

		return dx * dx + dy * dy;
	}

	bool operator <(const XYPair<T> &other) const
	{
		if(y < other.y)
			return true;
		else if(y > other.y)
		{
			return false;
		}
		else if(x < other.x)
		{
			return true;
		}
		else
		{
			return false;
		}
	}

	void MultiplyBy(int multiplication)
	{
		x *= multiplication;
		y *= multiplication;
	}

};

#endif