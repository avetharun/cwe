
#include <alib.h>
#include <imgui_uielement.h>
#include <mutex>

#include "engine/physics.h"
#pragma warning (disable: 4244)
std::mutex mutex;
std::vector<RectUnion*> _mGlobalRectArr = std::vector<RectUnion*>();
std::vector<LineUnion*> _mGlobalMeshArr = std::vector<LineUnion*>();
LineUnion* freestandingLinesMesh = new LineUnion();
CWRect CWLine::bounding_boxi(int sx, int sy, int ex, int ey) {
	CWRect _r = {
		alib_min(sx, ex) - 2,
		alib_min(sy, ey) - 2,
		alib_max(4,abs(ex - sx)),
		alib_max(4,abs(ey - sy))
	};
	return CWRect{
		static_cast<int>(_r.x), static_cast<int>(_r.y), static_cast<int>(_r.x), static_cast<int>(_r.y)
	};
}

CWRect  CWLine::bounding_box(CWLine m) {
	return bounding_boxi(m.start.x, m.start.y, m.end.x, m.end.y);
}

ImVec2  CWLine::midpoint() {
	return { (start.x + end.x) * 0.5f, (start.y + end.y) * 0.5f };
}

ImVec2  CWLine::midpoint(ImVec2  s, ImVec2  e) {
	return { (s.x + e.x) * 0.5f, (s.y + e.y) * 0.5f };
}

CWLine::CWLine(ImVec2  _s, ImVec2  _e) {
	this->start = _s; this->end = _e;
}

CWLine::CWLine(float x1, float y1, float x2, float y2) {
	this->start = { x1, y1 };
	this->end = { x2, y2 };
}


void CWLine::set_freestanding() {
	if (freestandingLinesMesh->lines.size() >= 6000) {
		printf("Standalone lines are slow, please consider creating a mesh collider instead!");
	}
	freestandingLinesMesh->lines.push_back(*this);
}
void CWLine::erase_freestanding() {
	alib_remove_any_of(freestandingLinesMesh->lines, *this);
}

LineUnion::LineUnion(std::vector<CWLine> Lines, int Layer) {
	lines = Lines; _mGlobalMeshArr.push_back(this);
}

LineUnion::LineUnion() {
	_mGlobalMeshArr.push_back(this);
}


void RectUnion::c_collider_hit(RectUnion* o) {
	//if (SUCCEEDED(&lua_colhit)) { lua_colhit.execute(o); }
}

RectUnion::RectUnion(std::vector<std::reference_wrapper<CWRect>> Rects, int Layer) {
	_mGlobalRectArr.push_back(this);
	layer = Layer;
}

RectUnion::RectUnion() {
	_mGlobalRectArr.push_back(this);
}

#pragma region RAYCASTING




bool Raycast2D::RectIntersects(CWRect* r1, CWRect* r2) {
	// Check if r1 is to the left of r2 or vice versa
	if (r1->x + r1->w < r2->x || r2->x + r2->w < r1->x)
		return false;

	// Check if r1 is above r2 or vice versa
	if (r1->y + r1->h < r2->y || r2->y + r2->h < r1->y)
		return false;

	// If neither condition is met, the rectangles intersect
	return true;
}

/* \brief Check if point (pos) is in rect (&rect)*/

bool Raycast2D::pointRect(CWRect* rect, ImVec2 pos)
{
	if (pos.x > rect->x && pos.x < rect->w && pos.y > rect->y && pos.y < rect->h)
		return true;

	return false;
}

// LINE/RECTANGLE

bool Raycast2D::lineRect(ImVec2 start, ImVec2 end, CWRect* rect) {
	int x1 = start.x, y1 = start.y, x2 = end.x, y2 = end.y;
	
	// check if the line has hit any of the rectangle's sides


	// uses the Line/Line function below
	bool left = lineLine(start.x, start.y, end.x, end.y, rect->x, rect->y, rect->x, rect->y + rect->h);
	bool right = lineLine(start.x, start.y, end.x, end.y, rect->x + rect->w, rect->y, rect->x + rect->w, rect->y + rect->h);
	bool top = lineLine(start.x, start.y, end.x, end.y, rect->x, rect->y, rect->x + rect->w, rect->y);
	bool bottom = lineLine(start.x, start.y, end.x, end.y, rect->x, rect->y + rect->h, rect->x + rect->w, rect->y + rect->h);
	// use conditionals to disable reassigning hit.pos (see lineLine)


	// if ANY of the above are true, the line
	// has hit the rectangle
	if (left || right || top || bottom) {
	return true;
	}
	return false;
}

bool Raycast2D::lineLine(float x1, float y1, float x2, float y2, float x3, float y3, float x4, float y4) {

	// calculate the direction of the lines
	float uA = ((x4 - x3) * (y1 - y3) - (y4 - y3) * (x1 - x3)) / ((y4 - y3) * (x2 - x1) - (x4 - x3) * (y2 - y1));
	float uB = ((x2 - x1) * (y1 - y3) - (y2 - y1) * (x1 - x3)) / ((y4 - y3) * (x2 - x1) - (x4 - x3) * (y2 - y1));

	// if uA and uB are between 0-1, lines are colliding
	if (uA >= 0 && uA <= 1 && uB >= 0 && uB <= 1) {

		// set hit.pos to where line hit
		float intersectionX = x1 + (uA * (x2 - x1));
		float intersectionY = y1 + (uA * (y2 - y1));
		RaycastHit::pos = ImVec2(intersectionX, intersectionY);
		return true;
	}
	return false;
}

bool Raycast2D::TestLineImplV(ImVec2 start1, ImVec2 end1, ImVec2 start2, ImVec2 end2) {
	return lineLine(start1.x, start1.y, end1.x, end1.y, start2.x, start2.y, end2.x, end2.y);
}

bool Raycast2D::TestLineImpl(CWLine first, CWLine second) {
	return lineLine(first.start.x, first.start.y, first.end.x, first.end.y, second.start.x, second.start.y, second.end.x, second.end.y);
}

/** \brief Test to see if line hits.
*/

bool Raycast2D::Test(ImVec2 start, ImVec2 end, int layer) {
	RaycastHit::hasHit = false;
	//SDL_SetRenderDrawColor(__phys_internal_renderer, 255, 64, 255, 255);
	//SDL_RenderDrawLine(__phys_internal_renderer, start.x, start.y, end.x, end.y);

	for (unsigned int i = 0; i < _mGlobalRectArr.size() && !RaycastHit::hasHit; i++) {
		RectUnion* _thiscol = _mGlobalRectArr[i];
		if (_thiscol->layer & layer || layer == -1) {
			//printf("Calculating rect with collider type %s : %d\n", coltohr(_mGlobalRectArr[i]->layer), i);
			for (CWRect rect : _thiscol->rects) {
				CWRect lr = CWLine::bounding_boxi(start.x, start.y, end.x, end.y);
				bool o = lineRect(start, end, &rect);
				if (o) {
					RaycastHit::object = _thiscol;
					RaycastHit::start = start;
					RaycastHit::end = end;
					RaycastHit::hasHit = true;
					*(&RaycastHit::col_id) = _thiscol->id;
					RaycastHit::layer = _thiscol->layer;
					return true;
				}
			}
		}
	}
	for (unsigned int li = 0; li < _mGlobalMeshArr.size(); li++) {
		LineUnion* m = _mGlobalMeshArr[li];
		for (size_t lo = 0; lo < m->lines.size(); lo++) {
			CWLine l = m->lines[lo];
			if (!(l.layer & layer) && (layer != -1)) {
				continue;
			}
			ImVec2 s = l.start;
			ImVec2 e = l.end;
			CWRect lr = CWLine::bounding_boxi(s.x, s.y, e.x, e.y);
			CWRect c_lr = CWLine::bounding_boxi(start.x, start.y, end.x, end.y);
			bool o = TestLineImplV(start, end, s, e);
			if (o) {
				RaycastHit::object = _mGlobalMeshArr[li];
				RaycastHit::start = start;
				RaycastHit::end = end;
				RaycastHit::hasHit = true;
				*(&RaycastHit::col_id) = l.id;
				RaycastHit::layer = l.layer;
				return true;
			}
		}
	}
	return false;
}

bool Raycast2D::TestExcept(ImVec2 start, ImVec2 end, int layer) {
	RaycastHit::hasHit = false;
	//SDL_SetRenderDrawColor(__phys_internal_renderer, 255, 64, 255, 255);
	//SDL_RenderDrawLine(__phys_internal_renderer, start.x, start.y, end.x, end.y);
	for (unsigned int i = 0; i < _mGlobalRectArr.size() && !RaycastHit::hasHit; i++) {
		RectUnion* _thiscol = _mGlobalRectArr[i];
		if (_thiscol->layer == layer || _thiscol->layer == COL_CENTER) {
			continue;
		}
		//printf("Calculating rect with collider type %s : %d\n", coltohr(_mGlobalRectArr[i]->layer), i);
		for (CWRect rect : _thiscol->rects) {
			CWRect lr = CWLine::bounding_boxi(start.x, start.y, end.x, end.y);
			bool o = lineRect(start, end, &rect);
			if (o) {
				RaycastHit::object = _thiscol;
				RaycastHit::start = start;
				RaycastHit::end = end;
				RaycastHit::hasHit = true;
				*(&RaycastHit::col_id) = _thiscol->id;
				RaycastHit::layer = _thiscol->layer;
				return true;
			}
		}
	}
	for (unsigned int li = 0; li < _mGlobalMeshArr.size(); li++) {
		LineUnion* m = _mGlobalMeshArr[li];
		for (size_t lo = 0; lo < m->lines.size(); lo++) {
			CWLine l = m->lines[lo];
			if (l.layer == layer || l.layer == COL_CENTER) {
				continue;
			}
			ImVec2 s = l.start;
			ImVec2 e = l.end;
			CWRect lr = CWLine::bounding_boxi(s.x, s.y, e.x, e.y);
			CWRect c_lr = CWLine::bounding_boxi(start.x, start.y, end.x, end.y);
			if (!CWRect::checkCollision(&c_lr, &lr)) { continue; }

			bool o = TestLineImplV(start, end, s, e);
			if (o) {
				RaycastHit::object = _mGlobalMeshArr[li];
				RaycastHit::start = start;
				RaycastHit::end = end;
				RaycastHit::hasHit = true;
				*(&RaycastHit::col_id) = l.id;
				RaycastHit::layer = l.layer;
				return true;
			}
		}
	}
	return false;
}

bool Raycast2D::TestRect(ImVec2 start, ImVec2 end, int layer) {
	RaycastHit::hasHit = false;
	//SDL_SetRenderDrawColor(__phys_internal_renderer, 255, 64, 255, 255);
	//SDL_RenderDrawLine(__phys_internal_renderer, start.x, start.y, end.x, end.y);
	for (unsigned int i = 0; i < _mGlobalRectArr.size() && !RaycastHit::hasHit; i++) {
		RectUnion* _thiscol = _mGlobalRectArr[i];
		if (_thiscol->layer & layer || layer == -1) {
			for (CWRect rect : _thiscol->rects) {
				CWRect lr = CWLine::bounding_boxi(start.x, start.y, end.x, end.y);
				bool o = lineRect(start, end, &rect);
				if (o) {
					RaycastHit::object = _thiscol;
					RaycastHit::start = start;
					RaycastHit::end = end;
					RaycastHit::hasHit = true;
					*(&RaycastHit::col_id) = _thiscol->id;
					RaycastHit::layer = _thiscol->layer;
					return true;
				}
			}
		}
	}
	for (unsigned int li = 0; li < _mGlobalMeshArr.size(); li++) {
		LineUnion* m = _mGlobalMeshArr[li];
		for (size_t lo = 0; lo < m->lines.size(); lo++) {
			CWLine l = m->lines[lo];
			if (!(l.layer & layer) && (layer != -1)) {
				continue;
			}
			ImVec2 s = l.start;
			ImVec2 e = l.end;
			CWRect lr = CWLine::bounding_boxi(s.x, s.y, e.x, e.y);
			CWRect c_lr = CWLine::bounding_boxi(start.x, start.y, end.x, end.y);

			bool o = CWRect::checkCollision(&c_lr, &lr);
			if (o) {
				RaycastHit::object = _mGlobalMeshArr[li];
				RaycastHit::start = start;
				RaycastHit::end = end;
				RaycastHit::hasHit = true;
				*(&RaycastHit::col_id) = l.id;
				RaycastHit::layer = l.layer;
				return true;
			}
		}
	}
	return false;
}

// This function name is counter-intuitive, it actually checks to see if rect R collides with anything *except* a specific collider. 
// This is used internally to check for player collisions.

bool Raycast2D::TestRectExcept(CWRect* rect, int layer, RectUnion* ignored_col) {
	RaycastHit::hasHit = false;
	//SDL_SetRenderDrawColor(__phys_internal_renderer, 255, 64, 255, 255);
	//SDL_RenderDrawLine(__phys_internal_renderer, start.x, start.y, end.x, end.y);
	for (unsigned int i = 0; i < _mGlobalRectArr.size() && !RaycastHit::hasHit; i++) {
		RectUnion* _thiscol = _mGlobalRectArr[i];
		if (ignored_col == _thiscol) { continue; }
		if (_thiscol->layer & layer || layer == -1) {
			for (CWRect rect1 : _thiscol->rects) {
				//printf("Calculating rect with collider type %s : %d\n", coltohr(_mGlobalRectArr[i]->layer), i);
				bool o = RectIntersects(&rect1, rect);
				if (o) {
					RaycastHit::object = _thiscol;
					RaycastHit::hasHit = true;
					RaycastHit::start = { (float)rect1.x, (float)rect1.y };
					RaycastHit::end = RaycastHit::start + ImVec2{ (float)rect1.w, (float)rect1.h };
					*(&RaycastHit::col_id) = _thiscol->id;
					RaycastHit::layer = _thiscol->layer;
					return true;
				}
			}
		}
	}
	for (unsigned int li = 0; li < _mGlobalMeshArr.size(); li++) {
		LineUnion* m = _mGlobalMeshArr[li];
		for (size_t lo = 0; lo < m->lines.size(); lo++) {
			CWLine l = m->lines[lo];
			if (!(l.layer & layer) && (layer != -1)) {
				continue;
			}
			ImVec2 s = l.start;
			ImVec2 e = l.end;
			CWRect lr = CWLine::bounding_boxi(s.x, s.y, e.x, e.y);

			bool o = lineRect(s, e, rect);
			if (o) {
				RaycastHit::object = _mGlobalMeshArr[li];
				RaycastHit::start = e;
				RaycastHit::end = s;
				RaycastHit::hasHit = true;
				*(&RaycastHit::col_id) = l.id;
				RaycastHit::layer = l.layer;
				return true;
			}
		}
	}
	return false;
}

bool Raycast2D::TestCircle(ImVec2 start, int diameter, int layer) {
	for (float a = 0; a < 360; a += (360.0f / (diameter * 2)) / 512.0f) {
		ImVec2 i(
			start.x + (diameter * cos(alib_deg2rad(a))),
			start.y + (diameter * sin(alib_deg2rad(a)))
		);
		if (Test(start, i, layer)) {
			return true;
		}
	}
	return false;
}

bool Raycast2D::TestCone(ImVec2 start, int angle, int offset, int dist, int layer) {
	for (float a = 0 - angle + offset; a < angle + offset; a += (360.0f + angle + offset) / 512.0f) {
		ImVec2 i(
			start.x + (dist * cos(alib_deg2rad(a))),
			start.y + (dist * sin(alib_deg2rad(a)))
		);
		if (Test(start, i, layer)) {
			return true;
		}
	}
	return false;
}

bool Raycast2D::TestAnyCone(ImVec2 start, int angle, int offset, int dist) {
	return TestCone(start, angle, offset, dist, -1);
}

bool Raycast2D::TestAnyLine(ImVec2 start, ImVec2 end) {
	return Test(start, end, -1);
}

bool Raycast2D::TestAnyCircle(ImVec2 start, int diameter) {
	return TestCircle(start, diameter, -1);
}

bool Raycast2D::TestConeExcept(ImVec2 start, int angle, int offset, int dist, int layer) {
	for (float a = 0 - angle + offset; a < angle + offset; a += (360.0f + angle + offset) / 512.0f) {
		ImVec2 i(
			start.x + (dist * cos(alib_deg2rad(a))),
			start.y + (dist * sin(alib_deg2rad(a)))
		);
		if (TestExcept(start, i, layer)) {
			return true;
		}
	}
	return false;
}

bool Raycast2D::TestCircleExcept(ImVec2 start, int diameter, int layer) {
	for (float a = 0; a < 360; a += (360.0f / (diameter * 2)) / 512.0f) {
		ImVec2 i(
			start.x + (diameter * cos(alib_deg2rad(a))),
			start.y + (diameter * sin(alib_deg2rad(a)))
		);
		if (TestExcept(start, i, layer)) {
			return true;
		}
	}
	return false;
}
#pragma endregion

bool CWRect::checkCollision(CWRect *_1, CWRect *_2) {
	return Raycast2D::RectIntersects(_1, _2);
}
