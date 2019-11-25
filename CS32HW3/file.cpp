class File
{
public:
	File(std::string name);
	std::string name() const;
	virtual void open() const = 0;
	virtual void redisplay() const;
	virtual ~File();
private:
	std::string m_name;
};

class TextMsg : public File
{
public:
	TextMsg(std::string name);
	virtual void open() const;
	virtual ~TextMsg();
};

class Picture : public File
{
public:
	Picture(std::string name);
	virtual void open() const;
	virtual ~Picture();
};

class Video : public File
{
public:
	Video(std::string name, int length);
	virtual void open() const;
	virtual void redisplay() const;
	virtual ~Video();
private:
	int m_length;
};

File::File(std::string name) : m_name(name) {}

std::string File::name() const 
{
	return m_name;
}

void File::redisplay() const
{
	std::cout << "refresh the screen";
}

File::~File() {}

TextMsg::TextMsg(std::string name) : File(name) {}
Picture::Picture(std::string name) : File(name) {}
Video::Video(std::string name, int length) : File(name),m_length(length) {}

void TextMsg::open() const
{
	std::cout << "open text message";
}
void Picture::open() const
{
	std::cout << "show picture";
}
void Video::open() const
{
	std::cout << "play " << m_length << " second video";
}

TextMsg::~TextMsg()
{
	std::cout << "Destroying " << name() << ", a text message" << std::endl;
}
Picture::~Picture()
{
	std::cout << "Destroying the picture " << name() << std::endl;
}
Video::~Video()
{
	std::cout << "Destroying " << name() << ", a video" << std::endl;
}

void Video::redisplay() const
{
	std::cout << "replay video";
}