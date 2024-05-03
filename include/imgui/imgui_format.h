#ifndef __htgnative_imgui_format_h_
#define __htgnative_imgui_format_h_
#pragma warning( push )

#pragma warning( disable : 4996 )
#pragma warning( disable : 4244 )
#include <string>
#include "imgui.h"
#include "imgui_internal.h"

namespace ImGui {
	/// <summary>
	/// HSV : Stub for a HSV (hue saturation value) triplet. Don't use, instead use ImRGB, as it contains functions to interperet this
	/// </summary>
	struct ImHSV {
		float hue, saturation, value;
	};
	/// <summary>
	/// HSL : Stub for a HSL (hue saturation luma) triplet. Don't use, instead use ImRGB, as it contains functions to interperet this
	/// Identical to ImHSV, except can convert to HSV, which is used instead of this.
	/// </summary>
	struct ImHSL {
		float hue, saturation, luma;
		operator ImHSV();
		ImHSL(ImHSV _hsv);
	};
	struct ImRGB {
		unsigned char colour[4] = { 255,255,255,255 };
		unsigned char getr();
		unsigned char getg();
		unsigned char getb();
		unsigned char geta();
		ImHSV convertHSV();
		void setr(unsigned char _r);
		void setg(unsigned char _g);
		void setb(unsigned char _b);
		void seta(unsigned char _a);
		void hue(float _h);
		void saturation(float _s);
		void value(float _v);
		void parse_hsv(float h, float s, float v);
		ImRGB(unsigned char r, unsigned char g, unsigned char b, unsigned char a = 255);
		ImRGB(ImVec4 c);
		ImRGB(ImHSV hsv);
		ImRGB() {}
		operator ImColor();
		//operator ImVec4() { return { (float)getr(), (float)getg(), (float)getb(), (float)geta() }; }
		operator ImU32();
		// Convert the color to a format understood by TextMulticolored()
		std::string tostring();
		// String to reset color
		static std::string resetstr();
	};
	ImRGB ColorConvertHSVtoRGB(ImHSV _hsv);
	ImHSV ColorConvertRGBtoHSV(ImRGB _rgb);

	bool __ProcessInlineHexColorImpl(const char* start, const char* end, ImVec4& color);

	void TextWrappedUnformatted(const char* text_begin, const char* text_end);

	void __TextColouredFormattedImplV(const char* fmt, va_list args);
	void __TextColouredFormattedImplUF(const char* fmt);
	void __TextColouredFormattedImpl(const char* fmt, ...);
	// Note: multicolored starts are formatted as follows BBGGRRAA. AA is optional.
	void TextMulticolored(const char* fmt, ...);
	void TextMulticoloredUnformatted(const char* text);
	// Note: multicolored starts are formatted as follows BBGGRRAA. AA is optional.
	void TextMulticoloured(const char* fmt, ...);
	// Aligns text to the right of screen, regardless of if there's text there already. Returns the position of the first element.
	ImVec2 TextAlignedRight(const char* fmt, ...);
	ImVec2 CalcTextAlignedRight(const char* fmt, ...);
};
typedef ImGui::ImRGB ImRGB;
typedef ImGui::ImHSL ImHSL;
typedef ImGui::ImHSV ImHSV;

#pragma warning( pop )

#endif