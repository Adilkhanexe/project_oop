#ifndef PaintToolH
#define PaintToolH
#include <Vcl.Graphics.hpp>

enum TToolType { TOOL_PEN, TOOL_LINE, TOOL_ERASER };

class TPaintTool
{
private:
	int lastX, lastY;
	int startX, startY;

    TToolType currentTool;
    TColor currentColor;
    int penSize;

public:
    __fastcall TPaintTool();

    // Setters
    void SetTool(TToolType tool) { currentTool = tool; }
    void SetColor(TColor color) { currentColor = color; }
    void SetPenSize(int NewSize) {
		if (NewSize > 0)
            penSize = NewSize;
	}

    TToolType GetCurrentTool() const { return currentTool; }
    TColor GetCurrentColor() const { return currentColor; }
    int GetPenSize() const { return penSize; }

    void MouseDown(int X, int Y);
    void MouseMove(TCanvas* Canvas, int X, int Y);
    void MouseUp(TCanvas* Canvas, int X, int Y);
};
#endif
