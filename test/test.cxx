

/* Examples of various aspects of C++ syntax */
namespace Example {

	struct Coord {
		int x;
		int y;
	};

	class Widget {
	
	protected:
		Widget() {}
	
	public:
		virtual ~Widget() {}

		/* Example of a pure virtual method */
		virtual int get_width() = 0;
		/* Reference: */
		void set_location(const struct Coord& coord);
	
	private:
		struct Coord m_location;
	};

	class Dialog : public Widget {
	
	public:
		Dialog(): Widget() {}
		~Dialog() {}
		int get_width() { return 400; }
	};
};

class test {
	
	int a, b;
	
	public:
		test (int, int);
		int somefunc (void);
};

test::test (int x, int y)
{
	a = x;
	b = y;
}

int test::somefunc (void)
{
	return a * b;
}


/* Example of a method definition, outside of the body of the class */
void Example::Widget::set_location(const struct Coord& coord)
{
	this->m_location = coord;
}

/* Examples of overloaded functions: */
void foo()
{
}

void foo(const char *str)
{
}

void foo(const Example::Coord& coord)
{
	/* Example of a call to a non-virtual method: */
	Example::Widget *dlg = new Example::Dialog();
	dlg->set_location(coord);
	delete dlg;
}

int main(int argc, char** argv){
  return 1;
}
