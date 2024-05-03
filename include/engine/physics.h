#pragma once
#include <string>
#include <functional>
#include <vector>
#include <type_traits>
#include <imgui.h>
#include <cmath>
#include <cstdint>
#include <cstdlib>
#include <memory>
#include <unordered_map>
#include <utility>
#include <map>

struct CWLayout;
#include <alib.h>
#include "cwabt.h"


/**
 * \brief Generic holder for an array of elements in a 2d grid.
 *			Each element can be associated with multiple positions- ie element has a width larger than GridSize
 * \see Quadtree \ Octree
 * \tparam GridSize - Size of each grid
 * \tparam T - Element type
 * 
 *
 */
template <typename T, int GridSize = 256>
struct GridMap {
	std::map<std::pair<size_t, size_t>, std::vector<std::reference_wrapper<T>>> elements;
	/// <summary>
	/// Converts a physical (world) size into virtual (pair) size
	/// </summary>
	/// <returns>Virtual size</returns>
	std::pair<size_t, size_t> getSizeForRelativeSize(int w, int h) {
		int gridSzX = 0;
		int gridSzY = 0;
		if (w != 0) {
			gridSzX = (int)ceil(w / GridSize);
		}
		if (h != 0) {
			gridSzY = (int)ceil(h / GridSize);
		}
		return std::make_pair(gridSzX, gridSzY);
	}
	/// <summary>
	/// Gets index for a given physical (world) position
	/// </summary>
	/// <returns></returns>
	std::pair<size_t, size_t> getIndexForRelativePosition(int x, int y) {
		int idxX = 0;
		int idxY = 0;
		// DVZ check:
		if (x != 0) { idxX = (int)floor(x / GridSize); }
		if (y != 0) { idxY = (int)floor(y / GridSize); }
		return std::make_pair(idxX, idxY);
	}
	bool containsAnything(int x, int y) {
		return elements.contains(getIndexForRelativePosition(x,y));
	}
	std::vector<std::reference_wrapper<T>> getContainedElements(int x, int y) {
		std::pair<int, int> index = getIndexForRelativePosition(x, y);
		bool bl2 = elements.contains(index);
		if (!bl2) { return std::vector<std::reference_wrapper<T>>(); }
		return elements[index];
	}
	std::vector<std::reference_wrapper<T>> getContainedElements(int x, int y, int w, int h) {
		std::pair<size_t, size_t> index = getIndexForRelativePosition(x, y);
		std::pair<size_t, size_t> size = getSizeForRelativeSize(w, h);
		std::vector<std::reference_wrapper<T>> vec = std::vector<std::reference_wrapper<T>>();
		for (int _x = index.first; _x <= index.first + size.first; _x++) {
			for (int _y = index.second; _y <= index.second + size.second; _y++) {
				std::pair<int, int> idx2 = { _x, _y };
				bool bl2 = elements.contains(idx2);
				if (!bl2) { continue; }
				std::vector<std::reference_wrapper<T>> gridvec = elements.at(idx2);
				for (std::reference_wrapper<T> elem : gridvec) {
					vec.push_back(elem);
				}
			}
		}
		return elements.at(index);
	}
private:
	/**
	 * .
	 * Never use! Meant for internal use ONLY!
	 * Will not assign to the reverse map, causing issues when trying to remove elements!
	 */
	void addElementsForPositions(std::vector<std::pair<size_t, size_t>> indexes, T& element) {
		for (auto& index : indexes) {
			elements[index].push_back(std::ref(element));
		}
	}
public:
	void addElement(int x, int y, T& element) {
		std::pair<size_t, size_t> index = getIndexForRelativePosition(x, y);
		elements[index].emplace_back(element);
	}
	void addElement(int x, int y, int w, int h, T& element) {
		std::vector<std::pair<size_t, size_t>> positions;
		std::pair<size_t, size_t> size = getSizeForRelativeSize(w, h);
		for (int _x = 0; _x <= size.first; _x++) {
			for (int _y = 0; _y <= size.second; _y++) {
				std::pair<int, int> index = std::make_pair(_x, _y);
				positions.push_back(index);
			}
		}
		addElementsForPositions(positions, element);
	}
	/**
	 * .
	 * 
	 * @param[in] element Element to remove. If it doesn't exist in the reverse map (which will happen if addElementsForPositions is used), we will try to find it regardless!
	 */
	void removeElement(T& element) {
		std::vector<std::pair<size_t, size_t>> positionsHoldingElement;
		for (auto& [k, v] : elements) {
			if (v == ref(element)) {
				positionsHoldingElement.push_back(k);
			}
		}
		for (std::pair<size_t, size_t> index : positionsHoldingElement) {
			int idx = 0;
			for (T& _element : elements.at(index)) {
				if (_element == ref(element)) {
					break;
				}
				idx++;
			}
			std::vector<T&> vec = elements.at(index);
			vec.erase(vec.begin()+idx);
		}
	}
};


struct CWRect {
	int x;
	int y;
	int w;
	int h;
	CWRect() {}
	CWRect(int _x, int _y, int _w, int _h): x(_x), y(_y), w(_w), h(_h) {}
	static bool checkCollision(CWRect* _1, CWRect* _2);
	ABTDataStub serialize();
	static CWRect deserialize(ABTDataStub compound);
};


struct CWLine {
	static CWRect bounding_boxi(int sx, int sy, int ex, int ey);
	static CWRect bounding_box(CWLine m);

	ImVec2 start = {};
	ImVec2 end = {};
	ImVec2 midpoint();
	static ImVec2 midpoint(ImVec2 s, ImVec2 e);
	int layer = 0;
	std::string id = "";
	static inline const int ID_LENGTH = 48;
	CWLine() {}
	CWLine(ImVec2 _s, ImVec2 _e);
	CWLine(float x1, float y1, float x2, float y2);
	void set_freestanding();
	void erase_freestanding();

	ABTDataStub serialize();
	static CWLine deserialize(ABTDataStub compound);

	bool operator==(const CWLine& other) const {
		return other.start.x == start.x && other.end.x == end.x && other.start.y == start.y && other.end.y == end.y;
	};
};


/// <summary>
/// A "Union" of lines. Used to group lines into an overall mesh.
/// </summary>
struct LineUnion {
	bool isCWLScriptable = false;
	std::function< void(CWLine) > OnColliderHit;

	void c_collider_hit(CWLine o);
	std::vector<CWLine> lines;
	LineUnion(std::vector<CWLine> Lines, int Layer = 1);
	LineUnion();
	void operator ~();
	ABTDataStub serialize();
	static LineUnion deserialize(ABTDataStub compound);
};

/// <summary>
/// A "Union" of rects. Used to group rects together into an overall structure. Only used for fetching via names
/// </summary>
struct RectUnion {
	std::vector<std::reference_wrapper<CWRect>> rects;
	std::string id = "";
	static inline const int ID_LENGTH = 48;
	int layer;

	static RectUnion* lu_new_fromr(std::vector<CWRect> r, int __layer);
	static RectUnion* lu_new_fromi(int x, int y, int w, int h, int __layer);

	static RectUnion* lu_new_fromic(int x, int y, int w, int h, int __layer);

	void c_collider_hit(RectUnion* o);

	RectUnion(std::vector<std::reference_wrapper<CWRect>> Rects, int Layer = 1);
	RectUnion();
	void operator ~();
	void free();
	ABTDataStub serialize();
	static RectUnion deserialize(ABTDataStub compound);
private:

	bool isStayState = false;
	bool isEnterState = false;
	bool isExitState = false;

public:


};
enum _collLayers : int {
	COL_EMPTY,
	COL_PLAYER,
	COL_CENTER,
	COL_STATIC,
	COL_ENT,
	COL_ITM,
	COL_OBJ,
	COL_TRG,
	COL_SOLID,
	COL_FUNC,
};
/**
 *  \brief Raycast a "line"
 *	\param start (vec2) : Where raycast starts
 *	\param end   (vec2) : Where raycast ends
 *
 *  \param ..
 */
struct Raycast2D {
	static bool RectIntersects(CWRect* r1, CWRect* r2);
	struct RaycastHit {
		static inline ImVec2 pos = {};
		static inline void* object = nullptr;
		static inline ImVec2 start = {};
		static inline ImVec2 end = {};
		static inline float length = 0;
		static inline bool hasHit = false;
		static inline std::string col_id = "";
		static inline int layer = 0;
	};

	/* \brief Check if point (pos) is in rect (&rect)*/
	static bool pointRect(CWRect* rect, ImVec2 pos);

	// visit http://www.jeffreythompson.org/collision-detection/line-rect.php for help, this site really nice, yknow
private:

	// LINE/RECTANGLE
	static bool lineRect(ImVec2 start, ImVec2 end, CWRect* rect);
	static bool lineLine(float x1, float y1, float x2, float y2, float x3, float y3, float x4, float y4);
public:
	static bool TestLineImplV(ImVec2 start1, ImVec2 end1, ImVec2 start2, ImVec2 end2);
	static bool TestLineImpl(CWLine first, CWLine second);
	/** \brief Test to see if line hits.
	*/
	static bool Test(ImVec2 start, ImVec2 end, int layer);;
	static bool TestExcept(ImVec2 start, ImVec2 end, int layer);;
	static bool TestRect(ImVec2 start, ImVec2 end, int layer);;
	// This function name is counter-intuitive, it actually checks to see if rect R collides with anything *except* a specific collider. 
	// This is used internally to check for player collisions.
	static bool TestRectExcept(CWRect* rect, int layer, RectUnion* ignored_col);;
	static bool TestCircle(ImVec2 start, int diameter, int layer);
	static bool TestCone(ImVec2 start, int angle, int offset, int dist, int layer);
	static bool TestAnyCone(ImVec2 start, int angle, int offset, int dist);
	static bool TestAnyLine(ImVec2 start, ImVec2 end);
	static bool TestAnyCircle(ImVec2 start, int diameter);
	static bool TestConeExcept(ImVec2 start, int angle, int offset, int dist, int layer);
	static bool TestCircleExcept(ImVec2 start, int diameter, int layer);
};



struct PhysWorld {
	std::unordered_map<std::string, std::reference_wrapper<LineUnion>> LineMeshes;
	std::unordered_map<std::string, std::reference_wrapper<RectUnion>> RectMeshes;
	// Begin processed(cached) elements
	// This will be used to hold all existing instances of a mesh, line, or rect.
	GridMap<CWLine> LINES;
	GridMap<CWRect> RECTS;
	void AddLine(CWLine& line) {
		int posX = (int)alib_min(line.start.x, line.end.x);
		int posY = (int)alib_min(line.start.y, line.end.y);
		int w = (int)std::abs(line.end.x - line.start.x); // Width
		int h = (int)std::abs(line.end.y - line.start.y); // Height
		LINES.addElement(posX, posY, w, h, line);
	}
	void AddRect(CWRect& rect) {
		RECTS.addElement(rect.x, rect.y, rect.w, rect.h, rect);
	}
	std::vector<CWLine> getLines(int x, int y) {
		std::vector<std::reference_wrapper<CWLine>> a = LINES.getContainedElements(x, y);
		std::vector<CWLine> lines;
		for (auto& elem : a) {
			lines.push_back(elem.get());
		}
		return lines;
	}
	std::vector<CWRect> getRects(int x, int y) {
		std::vector<std::reference_wrapper<CWRect>> a = RECTS.getContainedElements(x, y);
		std::vector<CWRect> rects;
		for (auto& elem : a) {
			rects.push_back(elem.get());
		}
		return rects;
	}
	std::vector<CWLine> getLines(int x, int y, int w, int h) {
		std::vector<std::reference_wrapper<CWLine>> a = LINES.getContainedElements(x, y, w, h);
		std::vector<CWLine> lines;
		for (auto& elem : a) {
			lines.push_back(elem.get());
		}
		return lines;
	}
	std::vector<CWRect> getRects(int x, int y, int w, int h) {
		std::vector<std::reference_wrapper<CWRect>> a = RECTS.getContainedElements(x, y, w, h);
		std::vector<CWRect> rects;
		for (auto& elem : a) {
			rects.push_back(elem.get());
		}
		return rects;
	}
};