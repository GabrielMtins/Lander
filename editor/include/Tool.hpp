#ifndef TOOL_HPP
#define TOOL_HPP

#include "Canvas.hpp"

class Tool {
	public:
		Tool(Canvas *parent);
		virtual void handleInput(Context *context);
		virtual void render(void);
		virtual void reset(void);
	
	protected:
		Canvas *parent;
};

#endif
