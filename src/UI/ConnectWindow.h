#pragma once

#include <UI/INuklearWindow.h>

namespace UI
{
	class ConnectWindow : public INuklearWindow
	{
	public:
		ConnectWindow(Int32 ID, const String& title, Bool visible)
		{
			mWindowID = ID;
			mWindowTitle = title;
			mVisible = visible;
		}

		void DrawWindow(nk_context* nuklearContext, Network::Protocol* protocolPtr)
		{
			if (!mVisible) return;

			if (nk_begin(nuklearContext, mWindowTitle.c_str(), nk_rect(50, 50, 400, 200), NK_WINDOW_MOVABLE | NK_WINDOW_MINIMIZABLE | NK_WINDOW_TITLE | NK_WINDOW_NO_SCROLLBAR))
			{
				static char nameInput[30];
				static char passInput[30];
				static int	textLength[2];

				nk_layout_row_dynamic(nuklearContext, 0, 2);
				nk_label(nuklearContext, "Usuario", NK_TEXT_LEFT);
				nk_edit_string(nuklearContext, NK_EDIT_FIELD, &nameInput[0], &textLength[0], 30, nk_filter_default);
				nk_label(nuklearContext, "Contraseña", NK_TEXT_LEFT);
				nk_edit_string(nuklearContext, NK_EDIT_FIELD, &passInput[0], &textLength[1], 30, nk_filter_default);

				nk_layout_row_dynamic(nuklearContext, 0, 2);
				if (nk_button_label(nuklearContext, "Crear Personaje"))
				{
					protocolPtr->WriteLoginNewChar(nameInput, passInput);
				}
				if (nk_button_label(nuklearContext, "Conectar"))
				{
					protocolPtr->Connect();
					protocolPtr->WriteLoginExistingChar(nameInput, passInput);
				}
			}
			nk_end(nuklearContext);
		}
	};
}