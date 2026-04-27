#include <vcl.h>
#pragma hdrstop
#include "PaintTool.h"
#pragma package(smart_init)
__fastcall TPaintTool::TPaintTool()
{

    lastX = 0;
    lastY = 0;
    startX = 0;
    startY = 0;
    currentTool = TOOL_PEN;
    currentColor = clBlack;
	penSize = 3;
}
void TPaintTool::MouseDown(int X, int Y)
{

    lastX = X;
    lastY = Y;
    startX = X;
    startY = Y;
}
void TPaintTool::MouseMove(TCanvas *Canvas, int X, int Y)
{
	// Рисуем только, если мышь зажата
    if (currentTool == TOOL_PEN || currentTool == TOOL_ERASER)
	{
        Canvas->Pen->Style = psSolid;

        if (currentTool == TOOL_PEN)
        {
            Canvas->Pen->Color = currentColor;
            Canvas->Pen->Width = penSize;
        }
        else if (currentTool == TOOL_ERASER)
		{
			Canvas->Pen->Color = clWhite;
            Canvas->Pen->Width = penSize + 5;
		}
        Canvas->MoveTo(lastX, lastY);
		Canvas->LineTo(X, Y);

        lastX = X;
        lastY = Y;
    }
}
void TPaintTool::MouseUp(TCanvas *Canvas, int X, int Y)
{
    if (currentTool == TOOL_LINE)
    {
        Canvas->Pen->Style = psSolid;
        Canvas->Pen->Color = currentColor;
		Canvas->Pen->Width = penSize;

        Canvas->MoveTo(startX, startY);
        Canvas->LineTo(X, Y);
    }
}
