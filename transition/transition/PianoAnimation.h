#pragma once
#include "AnimatorNode.h"
#include <tuple>
#include "GeometryNode.h"

class PianoAnimation : public AnimatorNode {

private:

#define BEAT 60.0/110.0
#define STEP 0.10

	Node* piano_;
	bool enabled_ = false;
	float last = -1;
	std::vector<std::tuple<double, bool, std::string>> events = {
		//Zwei Intro-B-Takte
		std::make_tuple(8 * BEAT, 1, "K_AB2"),
		std::make_tuple(8 * BEAT, 1, "K_F3"),
		std::make_tuple(8 * BEAT, 1, "K_AB3"),
		std::make_tuple(8 * BEAT, 1, "K_D4"),
		std::make_tuple(16 * BEAT, 0, "K_AB2"),
		std::make_tuple(16 * BEAT, 0, "K_F3"),
		std::make_tuple(16 * BEAT, 0, "K_AB3"),
		std::make_tuple(16 * BEAT, 0, "K_D4"),
		//1. Takt
		std::make_tuple(16 * BEAT, 1, "K_G1"),
		std::make_tuple(16 * BEAT, 1, "K_G4"),
		std::make_tuple(17 * BEAT - STEP, 0, "K_G1"),
		std::make_tuple(17 * BEAT, 1, "K_G2"),
		std::make_tuple(17 * BEAT, 1, "K_AB2"),
		std::make_tuple(17 * BEAT, 1, "K_D3"),
		std::make_tuple(19 * BEAT - STEP, 0, "K_G2"),
		std::make_tuple(19 * BEAT - STEP, 0, "K_AB2"),
		std::make_tuple(19 * BEAT - STEP, 0, "K_D3"),
		std::make_tuple(19 * BEAT, 0, "K_G4"),
		std::make_tuple(19 * BEAT, 1, "K_F4"),
		std::make_tuple(19 * BEAT, 1, "K_G2"),
		std::make_tuple(19 * BEAT, 1, "K_AB2"),
		std::make_tuple(19 * BEAT, 1, "K_D3"),
		std::make_tuple(19.5 * BEAT, 0, "K_F4"),
		std::make_tuple(19.5 * BEAT, 1, "K_DE4"),
		//2. Takt
		std::make_tuple(20 * BEAT, 0, "K_DE4"),
		std::make_tuple(20 * BEAT, 0, "K_G2"),
		std::make_tuple(20 * BEAT, 0, "K_AB2"),
		std::make_tuple(20 * BEAT, 0, "K_D3"),
		std::make_tuple(20 * BEAT, 1, "K_G1"),
		std::make_tuple(20 * BEAT, 1, "K_D4"),
		std::make_tuple(21 * BEAT, 0, "K_G1"),
		std::make_tuple(21 * BEAT, 1, "K_G2"),
		std::make_tuple(21 * BEAT, 1, "K_AB2"),
		std::make_tuple(21 * BEAT, 1, "K_D3"),
		std::make_tuple(23 * BEAT - STEP, 0, "K_G2"),
		std::make_tuple(23 * BEAT - STEP, 0, "K_AB2"),
		std::make_tuple(23 * BEAT - STEP, 0, "K_D3"),
		std::make_tuple(23 * BEAT, 0, "K_D4"),
		std::make_tuple(23 * BEAT, 1, "K_C4"),
		std::make_tuple(23 * BEAT, 1, "K_G2"),
		std::make_tuple(23 * BEAT, 1, "K_AB2"),
		std::make_tuple(23 * BEAT, 1, "K_D3"),
		//3. Takt
		std::make_tuple(24 * BEAT, 0, "K_C4"),
		std::make_tuple(24 * BEAT, 0, "K_G2"),
		std::make_tuple(24 * BEAT, 0, "K_AB2"),
		std::make_tuple(24 * BEAT, 0, "K_D3"),
		std::make_tuple(24 * BEAT, 1, "K_C2"),
		std::make_tuple(24 * BEAT, 1, "K_D4"),
		std::make_tuple(25 * BEAT, 0, "K_C2"),
		std::make_tuple(25 * BEAT, 1, "K_G2"),
		std::make_tuple(25 * BEAT, 1, "K_C3"),
		std::make_tuple(25 * BEAT, 1, "K_DE3"),
		std::make_tuple(27 * BEAT - STEP, 0, "K_G2"),
		std::make_tuple(27 * BEAT - STEP, 0, "K_C3"),
		std::make_tuple(27 * BEAT - STEP, 0, "K_DE3"),
		std::make_tuple(27 * BEAT, 1, "K_G2"),
		std::make_tuple(27 * BEAT, 1, "K_C3"),
		std::make_tuple(27 * BEAT, 1, "K_DE3"),
		std::make_tuple(28 * BEAT, 0, "K_G2"),
		std::make_tuple(28 * BEAT, 0, "K_C3"),
		std::make_tuple(28 * BEAT, 0, "K_DE3"),
		std::make_tuple(28 * BEAT, 0, "K_D4"),

		//Unsichtbar

		//9. Takt
		std::make_tuple(48 * BEAT, 1, "K_AB1"),
		std::make_tuple(48 * BEAT, 1, "K_D4"),
		std::make_tuple(49 * BEAT, 0, "K_AB1"),
		std::make_tuple(49 * BEAT, 1, "K_AB2"),
		std::make_tuple(49 * BEAT, 1, "K_D3"),
		std::make_tuple(49 * BEAT, 1, "K_F3"),
		std::make_tuple(51 * BEAT - STEP, 0, "K_AB2"),
		std::make_tuple(51 * BEAT - STEP, 0, "K_D3"),
		std::make_tuple(51 * BEAT - STEP, 0, "K_F3"),
		std::make_tuple(51 * BEAT, 1, "K_AB2"),
		std::make_tuple(51 * BEAT, 1, "K_D3"),
		std::make_tuple(51 * BEAT, 1, "K_F3"),
		//10. Takt
		std::make_tuple(52 * BEAT, 0, "K_D4"),
		std::make_tuple(52 * BEAT, 0, "K_AB2"),
		std::make_tuple(52 * BEAT, 0, "K_D3"),
		std::make_tuple(52 * BEAT, 0, "K_F3"),
		std::make_tuple(52 * BEAT, 1, "K_AB1"),
		std::make_tuple(52.5 * BEAT, 1, "K_F5"),
		std::make_tuple(53 * BEAT, 0, "K_AB1"),
		std::make_tuple(53 * BEAT, 1, "K_AB2"),
		std::make_tuple(53 * BEAT, 1, "K_D3"),
		std::make_tuple(53 * BEAT, 1, "K_F3"),
		std::make_tuple(53.0 * BEAT, 0, "K_F5"),
		std::make_tuple(53.0 * BEAT, 1, "K_DE5"),
		std::make_tuple(53.5 * BEAT, 0, "K_DE5"),
		std::make_tuple(53.5 * BEAT, 1, "K_D5"),
		std::make_tuple(54.0 * BEAT, 0, "K_D5"),
		std::make_tuple(54.0 * BEAT, 1, "K_C5"),
		std::make_tuple(54.5 * BEAT, 0, "K_C5"),
		std::make_tuple(54.5 * BEAT, 1, "K_AB4"),
		std::make_tuple(55 * BEAT - STEP, 0, "K_AB2"),
		std::make_tuple(55 * BEAT - STEP, 0, "K_D3"),
		std::make_tuple(55 * BEAT - STEP, 0, "K_F3"),
		std::make_tuple(55.0 * BEAT, 0, "K_AB4"),
		std::make_tuple(55 * BEAT, 1, "K_AB2"),
		std::make_tuple(55 * BEAT, 1, "K_D3"),
		std::make_tuple(55 * BEAT, 1, "K_F3"),
		std::make_tuple(55.0 * BEAT, 1, "K_A4"),
		std::make_tuple(55.5 * BEAT, 0, "K_A4"),
		std::make_tuple(55.5 * BEAT, 1, "K_G4"),
		//11. Takt
		std::make_tuple(56 * BEAT, 0, "K_G4"),
		std::make_tuple(56 * BEAT, 0, "K_AB2"),
		std::make_tuple(56 * BEAT, 0, "K_D3"),
		std::make_tuple(56 * BEAT, 0, "K_F3"),
		std::make_tuple(56 * BEAT, 1, "K_D2"),
		std::make_tuple(56 * BEAT, 1, "K_FG4"),
		std::make_tuple(57 * BEAT, 0, "K_D2"),
		std::make_tuple(57 * BEAT, 0, "K_FG4"),
		//Unsichtbar
		//12. Takt
		std::make_tuple(60 * BEAT, 1, "K_D2"),
		std::make_tuple(61 * BEAT, 0, "K_D2"),
		//Sichtbar
		//15. Takt
		std::make_tuple(72 * BEAT, 1, "K_C2"),
		std::make_tuple(72 * BEAT, 1, "K_D4"),
		std::make_tuple(73 * BEAT, 0, "K_C2"),
		std::make_tuple(73 * BEAT, 1, "K_G2"),
		std::make_tuple(73 * BEAT, 1, "K_C3"),
		std::make_tuple(73 * BEAT, 1, "K_DE3"),
		std::make_tuple(75 * BEAT - STEP, 0, "K_G2"),
		std::make_tuple(75 * BEAT - STEP, 0, "K_C3"),
		std::make_tuple(75 * BEAT - STEP, 0, "K_DE3"),
		std::make_tuple(75 * BEAT, 1, "K_G2"),
		std::make_tuple(75 * BEAT, 1, "K_C3"),
		std::make_tuple(75 * BEAT, 1, "K_DE3"),
		//16. Takt
		std::make_tuple(76 * BEAT, 0, "K_G2"),
		std::make_tuple(76 * BEAT, 0, "K_C3"),
		std::make_tuple(76 * BEAT, 0, "K_DE3"),
		std::make_tuple(76 * BEAT, 0, "K_D4"),
		std::make_tuple(76 * BEAT, 1, "K_C2"),
		std::make_tuple(76.5 * BEAT, 1, "K_C4"),
		std::make_tuple(77.0 * BEAT, 0, "K_C4"),
		std::make_tuple(77 * BEAT, 0, "K_C2"),
		std::make_tuple(77 * BEAT, 1, "K_G2"),
		std::make_tuple(77 * BEAT, 1, "K_C3"),
		std::make_tuple(77 * BEAT, 1, "K_DE3"),
		std::make_tuple(77.5 * BEAT, 1, "K_D4"),
		std::make_tuple(78.0 * BEAT, 0, "K_D4"),
		std::make_tuple(78.0 * BEAT, 1, "K_DE4"),
		std::make_tuple(78.5 * BEAT, 0, "K_DE4"),
		std::make_tuple(78.5 * BEAT, 1, "K_F4"),
		std::make_tuple(79 * BEAT - STEP, 0, "K_G2"),
		std::make_tuple(79 * BEAT - STEP, 0, "K_C3"),
		std::make_tuple(79 * BEAT - STEP, 0, "K_DE3"),
		std::make_tuple(79 * BEAT, 1, "K_G2"),
		std::make_tuple(79 * BEAT, 1, "K_C3"),
		std::make_tuple(79 * BEAT, 1, "K_DE3"),
		std::make_tuple(79.0 * BEAT, 0, "K_F4"),
		std::make_tuple(79.0 * BEAT, 1, "K_DE4"),
		std::make_tuple(79.5 * BEAT, 0, "K_DE4"),
		std::make_tuple(79.5 * BEAT, 1, "K_C4"),
		//17. Takt
		std::make_tuple(80 * BEAT, 0, "K_C4"),
		std::make_tuple(80 * BEAT, 0, "K_G2"),
		std::make_tuple(80 * BEAT, 0, "K_C3"),
		std::make_tuple(80 * BEAT, 0, "K_DE3"),
		std::make_tuple(80 * BEAT, 1, "K_D2"),
		std::make_tuple(80 * BEAT, 1, "K_D4"),
		std::make_tuple(81 * BEAT, 0, "K_D2"),
		std::make_tuple(81 * BEAT, 1, "K_A2"),
		std::make_tuple(81 * BEAT, 1, "K_D3"),
		std::make_tuple(81 * BEAT, 1, "K_FG3"),
		std::make_tuple(83 * BEAT - STEP, 0, "K_A2"),
		std::make_tuple(83 * BEAT - STEP, 0, "K_D3"),
		std::make_tuple(83 * BEAT - STEP, 0, "K_FG3"),
		std::make_tuple(83 * BEAT, 1, "K_A2"),
		std::make_tuple(83 * BEAT, 1, "K_D3"),
		std::make_tuple(83 * BEAT, 1, "K_FG3"),
		//18. Takt
		std::make_tuple(84 * BEAT, 0, "K_A2"),
		std::make_tuple(84 * BEAT, 0, "K_D3"),
		std::make_tuple(84 * BEAT, 0, "K_FG3"),
		std::make_tuple(84 * BEAT, 0, "K_D4"),
		std::make_tuple(84 * BEAT, 1, "K_D2"),
		std::make_tuple(85 * BEAT, 0, "K_D2"),
		std::make_tuple(85 * BEAT, 1, "K_A2"),
		std::make_tuple(85 * BEAT, 1, "K_D3"),
		std::make_tuple(85 * BEAT, 1, "K_FG3"),
		std::make_tuple(85.5 * BEAT, 1, "K_D4"),
		std::make_tuple(86.0 * BEAT, 0, "K_D4"),
		std::make_tuple(86.0 * BEAT, 1, "K_DE4"),
		std::make_tuple(86.5 * BEAT, 0, "K_DE4"),
		std::make_tuple(86.5 * BEAT, 1, "K_FG4"),
		std::make_tuple(87 * BEAT - STEP, 0, "K_A2"),
		std::make_tuple(87 * BEAT - STEP, 0, "K_D3"),
		std::make_tuple(87 * BEAT - STEP, 0, "K_FG3"),
		std::make_tuple(87 * BEAT, 1, "K_A2"),
		std::make_tuple(87 * BEAT, 1, "K_D3"),
		std::make_tuple(87 * BEAT, 1, "K_FG3"),
		std::make_tuple(87.0 * BEAT, 0, "K_FG4"),
		std::make_tuple(87.0 * BEAT, 1, "K_G4"),
		std::make_tuple(87.5 * BEAT, 0, "K_G4"),
		std::make_tuple(87.5 * BEAT, 1, "K_A4"),
		//19. Takt 
		std::make_tuple(88 * BEAT, 0, "K_A4"),
		std::make_tuple(88 * BEAT, 0, "K_A2"),
		std::make_tuple(88 * BEAT, 0, "K_D3"),
		std::make_tuple(88 * BEAT, 0, "K_FG3"),
		std::make_tuple(88 * BEAT, 1, "K_G1"),
		std::make_tuple(88 * BEAT, 1, "K_G4"),
		std::make_tuple(89 * BEAT, 0, "K_G1"),
		std::make_tuple(89 * BEAT, 0, "K_G4"),
		std::make_tuple(89 * BEAT, 1, "K_G2"),
		std::make_tuple(89 * BEAT, 1, "K_AB2"),
		std::make_tuple(89 * BEAT, 1, "K_D3"),
		std::make_tuple(89.5 * BEAT, 1, "K_AB4"),
		std::make_tuple(90.0 * BEAT, 0, "K_AB4"),
		std::make_tuple(90.0 * BEAT, 1, "K_C5"),
		std::make_tuple(90.5 * BEAT, 0, "K_C5"),
		std::make_tuple(90.5 * BEAT, 1, "K_D5"),
		std::make_tuple(91 * BEAT - STEP, 0, "K_G2"),
		std::make_tuple(91 * BEAT - STEP, 0, "K_AB2"),
		std::make_tuple(91 * BEAT - STEP, 0, "K_D3"),
		std::make_tuple(91 * BEAT, 1, "K_G2"),
		std::make_tuple(91 * BEAT, 1, "K_AB2"),
		std::make_tuple(91 * BEAT, 1, "K_D3"),
		std::make_tuple(91.0 * BEAT, 0, "K_D5"),
		std::make_tuple(91.0 * BEAT, 1, "K_C5"),
		std::make_tuple(91.5 * BEAT, 0, "K_C5"),
		std::make_tuple(91.5 * BEAT, 1, "K_AB4"),
		//20. Takt
		std::make_tuple(92.0 * BEAT, 0, "K_AB4"),
		std::make_tuple(92 * BEAT, 0, "K_G2"),
		std::make_tuple(92 * BEAT, 0, "K_AB2"),
		std::make_tuple(92 * BEAT, 0, "K_D3"),
		std::make_tuple(92 * BEAT, 1, "K_G1"),
		std::make_tuple(92.0 * BEAT, 1, "K_A4"),
		std::make_tuple(92.5 * BEAT, 0, "K_A4"),
		std::make_tuple(92.5 * BEAT, 1, "K_AB4"),
		std::make_tuple(93.0 * BEAT, 0, "K_AB4"),
		std::make_tuple(93 * BEAT, 0, "K_G1"),
		std::make_tuple(93 * BEAT, 1, "K_G2"),
		std::make_tuple(93 * BEAT, 1, "K_AB2"),
		std::make_tuple(93 * BEAT, 1, "K_D3"),
		std::make_tuple(93.0 * BEAT, 1, "K_A4"),
		std::make_tuple(93.5 * BEAT, 0, "K_A4"),
		std::make_tuple(93.5 * BEAT, 1, "K_G4"),
		std::make_tuple(94.0 * BEAT, 0, "K_G4"),
		std::make_tuple(94.0 * BEAT, 1, "K_A4"),
		std::make_tuple(94.5 * BEAT, 0, "K_A4"),
		std::make_tuple(94.5 * BEAT, 1, "K_G4"),
		std::make_tuple(95 * BEAT - STEP, 0, "K_G2"),
		std::make_tuple(95 * BEAT - STEP, 0, "K_AB2"),
		std::make_tuple(95 * BEAT - STEP, 0, "K_D3"),
		std::make_tuple(95 * BEAT, 1, "K_G2"),
		std::make_tuple(95 * BEAT, 1, "K_AB2"),
		std::make_tuple(95 * BEAT, 1, "K_D3"),
		std::make_tuple(95.0 * BEAT, 0, "K_G4"),
		std::make_tuple(95.0 * BEAT, 1, "K_F4"),
		std::make_tuple(95.5 * BEAT, 0, "K_F4"),
		std::make_tuple(95.5 * BEAT, 1, "K_E4"),
		//21. Takt
		std::make_tuple(96.0 * BEAT, 0, "K_E4"),
		std::make_tuple(96 * BEAT, 0, "K_G2"),
		std::make_tuple(96 * BEAT, 0, "K_AB2"),
		std::make_tuple(96 * BEAT, 0, "K_D3"),
		std::make_tuple(96 * BEAT, 1, "K_C2"),
		std::make_tuple(96 * BEAT, 1, "K_D4"),
		std::make_tuple(97 * BEAT, 0, "K_C2"),
		std::make_tuple(97 * BEAT, 1, "K_G2"),
		std::make_tuple(97 * BEAT, 1, "K_C3"),
		std::make_tuple(97 * BEAT, 1, "K_DE3"),
		std::make_tuple(99 * BEAT - STEP, 0, "K_G2"),
		std::make_tuple(99 * BEAT - STEP, 0, "K_C3"),
		std::make_tuple(99 * BEAT - STEP, 0, "K_DE3"),
		std::make_tuple(99 * BEAT, 1, "K_G2"),
		std::make_tuple(99 * BEAT, 1, "K_C3"),
		std::make_tuple(99 * BEAT, 1, "K_DE3"),
		//22. Takt
		std::make_tuple(100 * BEAT, 0, "K_D4"),
		std::make_tuple(100 * BEAT, 0, "K_G2"),
		std::make_tuple(100 * BEAT, 0, "K_C3"),
		std::make_tuple(100 * BEAT, 0, "K_DE3"),
		std::make_tuple(100 * BEAT, 1, "K_C2"),
		std::make_tuple(100.5 * BEAT, 1, "K_D4"),
		std::make_tuple(101.0 * BEAT, 0, "K_D4"),
		std::make_tuple(101 * BEAT, 0, "K_C2"),
		std::make_tuple(101 * BEAT, 1, "K_G2"),
		std::make_tuple(101 * BEAT, 1, "K_C3"),
		std::make_tuple(101 * BEAT, 1, "K_DE3"),
		std::make_tuple(101.0 * BEAT, 1, "K_C4"),
		std::make_tuple(101.5 * BEAT, 0, "K_C4"),
		std::make_tuple(101.5 * BEAT, 1, "K_AB3"),
		std::make_tuple(102.0 * BEAT, 0, "K_AB3"),
		std::make_tuple(102.0 * BEAT, 1, "K_A3"),
		std::make_tuple(102.5 * BEAT, 0, "K_A3"),
		std::make_tuple(102.5 * BEAT, 1, "K_G3"),
		std::make_tuple(103 * BEAT - STEP, 0, "K_G2"),
		std::make_tuple(103 * BEAT - STEP, 0, "K_C3"),
		std::make_tuple(103 * BEAT - STEP, 0, "K_DE3"),
		std::make_tuple(103.0 * BEAT, 0, "K_G3"),
		std::make_tuple(103 * BEAT, 1, "K_G2"),
		std::make_tuple(103 * BEAT, 1, "K_C3"),
		std::make_tuple(103 * BEAT, 1, "K_DE3"),
		std::make_tuple(103.0 * BEAT, 1, "K_A3"),
		std::make_tuple(103.5 * BEAT, 0, "K_A3"),
		std::make_tuple(103.5 * BEAT, 1, "K_AB3"),
		//23. Takt
		std::make_tuple(104 * BEAT, 0, "K_AB3"),
		std::make_tuple(104 * BEAT, 0, "K_G2"),
		std::make_tuple(104 * BEAT, 0, "K_C3"),
		std::make_tuple(104 * BEAT, 0, "K_DE3"),
		std::make_tuple(104 * BEAT, 1, "K_AB1"),
		std::make_tuple(104 * BEAT, 1, "K_C4"),
		std::make_tuple(105 * BEAT, 0, "K_AB1"),
		std::make_tuple(105 * BEAT, 1, "K_F2"),
		std::make_tuple(105 * BEAT, 1, "K_AB2"),
		std::make_tuple(105 * BEAT, 1, "K_D3"),
		std::make_tuple(107 * BEAT - STEP, 0, "K_F2"),
		std::make_tuple(107 * BEAT - STEP, 0, "K_AB2"),
		std::make_tuple(107 * BEAT - STEP, 0, "K_D3"),
		std::make_tuple(107 * BEAT, 1, "K_F2"),
		std::make_tuple(107 * BEAT, 1, "K_AB2"),
		std::make_tuple(107 * BEAT, 1, "K_D3"),
		//24. Takt (done)
		std::make_tuple(108 * BEAT, 0, "K_F2"),
		std::make_tuple(108 * BEAT, 0, "K_AB2"),
		std::make_tuple(108 * BEAT, 0, "K_D3"),
		std::make_tuple(108 * BEAT, 0, "K_C4")

	};

	void do_event(bool down, const std::string& key) {
		GeometryNode* node = (GeometryNode*)piano_->find_by_name(key + "_0");
		node->apply_transformation(Transformation::rotate_around_point(5*(2*down-1), glm::vec3(0, 0, 1), node->get_position()));
	}

public:

	explicit PianoAnimation(std::string name, Node* piano) : AnimatorNode(name)
	{
		this->piano_ = piano;
	}

	void start_if_not_automatic() override {
		enabled_ = true;
	}

	void update(double delta) override {
		if (!enabled_) return;
		float now;
		if (last == -1) {
			now = 0;
		}
		else {
			now = last + delta;
		}
		int count = 0;
		for (auto pevent : events) {
			double time = std::get<0>(pevent);
			if (time <= now && time > last) {
				do_event(std::get<1>(pevent), std::get<2>(pevent));
			}
			else if (time > now) {
				break;
			}
			++count;
		}
		if (count > 0) {
			events.erase(events.begin(), events.begin() + count - 1);
		}
		last = now;
	}

};