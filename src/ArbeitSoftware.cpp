// Application Header
#include "ArbeitSoftware.h"
#include "alternative.h"
#include <cassert>

//
//  Test
//
class Test {
	std::string value;
public:
    // Ctor
	explicit Test(std::string a) {
		value = std::move(a);
		std::cout << "A-" << value;
	}
    // Dtor
	~Test() {
		std::cout << "B-" << value;
	}
};

//
//  Ancestor
//
class Ancestor {
public:
	void Write() {
		std::cout << "AncestorWrite" << '\n';
	}
	virtual void Write(const std::string& text) {
		std::cout << text << '\n';
	}
};

//
//  Child
//
class Child : public Ancestor {
public:
	void Write(const std::string& text) override {
		std::cout << "Child: " << text << '\n';
		Ancestor::Write(text);
	}
};

//
//  f
//
auto f(const std::vector<int>& a, const std::vector<int>& b)
{
	std::vector<int> r(a.begin(), a.end());
	std::for_each(b.begin(), b.end(), [&](const auto& e) {
		auto it = std::lower_bound(r.begin(), r.end(), e);
		r.insert(it, e);
	});
	return r;
}

//
//  fn
//
auto fn(std::vector<int>& v)
{
    std::nth_element(v.begin(), v.begin()+1, v.end(), std::greater<>());
    return v[1];
}

//
//  func
//
Test* func(Test* t)
{
    std::shared_ptr<Test> t1(new Test("1"));
    std::unique_ptr<Test> t2(new Test("2"));

    Test* t3 = new Test("3");
    t = t1.get();
    return t3;
}

//
//  Problem2
//
void Problem2()
{
    auto* c = new Child();
    c->Write("Text");
    dynamic_cast<Ancestor*>(c)->Write();
}

//  C (for "charge"): Double the beam's strength.
//  S (for "shoot"): Shoot the beam, doing damage equal to the beam's current strength.

//  For example, if the robot's program is SCCSSC, the robot will do the following when the program runs:

//  Shoot the beam, doing 1 damage.
//  Charge the beam, doubling the beam's strength to 2.
//  Charge the beam, doubling the beam's strength to 4.
//  Shoot the beam, doing 4 damage.
//  Shoot the beam, doing 4 damage.
//  Charge the beam, increasing the beam's strength to 8.

//  In that case, the program would do a total of 9 damage.

int ComputeDamage(const std::vector<char>& v)
{
    int damage{0}, total_damage{0}, strength{1};

    for (auto& c : v) {
        if (c == 'S'/*Shoot the beam*/) {
            damage = 1 * strength;
            total_damage += damage;
        }
        else if (c == 'C'/*Charge the beam*/) {
            strength = 2 * strength;
        }
    }
    return total_damage;
}

int AlienRobot(std::vector<char>& v, int shield)
{
    // Compute damage for sequence in its current state
    auto damage = ComputeDamage(v);

    // Shield will resist
    if (damage <= shield) {return -1;} //COMMENT - In this case we didn't need a single swap to save so the minimum number of swaps required is 0. -1 Was the value we agreed to represent "impossible"

    // Pattern to be found
    const std::vector p{'C','S'};
    // Number Of iterations
    int iterations{0};

    do {
        // Search for an adjacent match
        auto it = std::search(v.begin(), v.end(), p.begin(), p.end()); //COMMENT - It searches left to right but we saw that we should swap first the right-most CS
        // Adjacent match found
        if (it != v.end()) {
            // Swap 'C' with 'S'
            if (it + 1 != v.end()) {
               *it = 'S'; *(it+1) = 'C';
            }
            // Re-compute damage for new sequence
            damage = ComputeDamage(v);
        }
        // Increment iterations
        ++iterations;
    } while (damage > shield); //COMMENT - It's possible that we can't save earth because after all possible swaps the damage is still higher than shield. This while will become an endless loop in this case

    return iterations;
}

void validateAlternative()
{
    assert(alternative::saveEarth("S", 3) == 0);
    assert(alternative::saveEarth("S", 1) == 0);
    assert(alternative::saveEarth("S", 0) == -1);
    assert(alternative::saveEarth("C", 0) == 0);
    assert(alternative::saveEarth("C", 10) == 0);

    assert(alternative::saveEarth("CC", 1) == 0);
    assert(alternative::saveEarth("CC", 0) == 0);
    assert(alternative::saveEarth("CC", 50) == 0);

    assert(alternative::saveEarth("SS", 1) == -1);
    assert(alternative::saveEarth("SS", 0) == -1);
    assert(alternative::saveEarth("SS", 2) == 0);
    assert(alternative::saveEarth("SS", 3) == 0);
    assert(alternative::saveEarth("SS", 50) == 0);

    assert(alternative::saveEarth("SC", 1) == 0);
    assert(alternative::saveEarth("SC", 0) == -1);
    assert(alternative::saveEarth("SC", 2) == 0);
    assert(alternative::saveEarth("SC", 50) == 0);

    assert(alternative::saveEarth("CS", 0) == -1);
    assert(alternative::saveEarth("CS", 1) == 1);
    assert(alternative::saveEarth("CS", 2) == 0);
    assert(alternative::saveEarth("CS", 3) == 0);
    assert(alternative::saveEarth("CS", 50) == 0);

    assert(alternative::saveEarth("CCC", 3) == 0);
    assert(alternative::saveEarth("CCC", 0) == 0);
    assert(alternative::saveEarth("CCC", 50) == 0);
    assert(alternative::saveEarth("CCC", 1) == 0);

    assert(alternative::saveEarth("SSS", 0) == -1);
    assert(alternative::saveEarth("SSS", 1) == -1);
    assert(alternative::saveEarth("SSS", 2) == -1);
    assert(alternative::saveEarth("SSS", 3) == 0);
    assert(alternative::saveEarth("SSS", 4) == 0);
    assert(alternative::saveEarth("SSS", 50) == 0);

    assert(alternative::saveEarth("SSC", 3) == 0);
    assert(alternative::saveEarth("SSC", 2) == 0);
    assert(alternative::saveEarth("SSC", 1) == -1);
    assert(alternative::saveEarth("SSC", 0) == -1);
    assert(alternative::saveEarth("SSC", 50) == 0);

    assert(alternative::saveEarth("SCS", 3) == 0);
    assert(alternative::saveEarth("SCS", 50) == 0);
    assert(alternative::saveEarth("SCS", 1) == -1);
    assert(alternative::saveEarth("SCS", 0) == -1);
    assert(alternative::saveEarth("SCS", 2) == 1);

    assert(alternative::saveEarth("CSS", 4) == 0);
    assert(alternative::saveEarth("CSS", 50) == 0);
    assert(alternative::saveEarth("CSS", 3) == 1);
    assert(alternative::saveEarth("CSS", 2) == 2);
    assert(alternative::saveEarth("CSS", 1) == -1);
    assert(alternative::saveEarth("CSS", 0) == -1);

    assert(alternative::saveEarth("SCC", 2) == 0);
    assert(alternative::saveEarth("SCC", 50) == 0);
    assert(alternative::saveEarth("SCC", 1) == 0);
    assert(alternative::saveEarth("SCC", 0) == -1);

    assert(alternative::saveEarth("CSC", 2) == 0);
    assert(alternative::saveEarth("CSC", 50) == 0);
    assert(alternative::saveEarth("CSC", 1) == 1);
    assert(alternative::saveEarth("CSC", 0) == -1);

    assert(alternative::saveEarth("CCS", 4) == 0);
    assert(alternative::saveEarth("CCS", 50) == 0);
    assert(alternative::saveEarth("CCS", 3) == 1);
    assert(alternative::saveEarth("CCS", 2) == 1);
    assert(alternative::saveEarth("CCS", 1) == 2);
    assert(alternative::saveEarth("CCS", 0) == -1);

    assert(alternative::saveEarth("SCCSSC", 6) == 2);
    assert(alternative::saveEarth("CSCSS", 3) == 5);
}

//
//  Main
//
int main()
{
    //testing the alternative
    validateAlternative();
    /*std::vector v{'S','C','C','S','S','C'};
    auto iterations = AlienRobot(v, 5);

	std::vector<int> a{1,2,4,5,7};
	std::vector<int> b{2,3,5,6};
	auto r = f(a, b);

	std::vector<int> c{1,3,4,2,5};
	auto d = fn(c);

	Problem2();

    Test* t4 = new Test("4");
    Test* t3 = func(t4);

    delete t3;
    delete t4;*/

	return 0;
}
