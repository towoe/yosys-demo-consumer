#include "kernel/yosys.h"

//Sample module
static Yosys::RTLIL::Module *adder(Yosys::RTLIL::Design *d, int width = 4) {
	Yosys::RTLIL::Module *m = d->addModule("\\adder");
	Yosys::RTLIL::Wire *a = m->addWire("\\a", width);
	Yosys::RTLIL::Wire *b = m->addWire("\\b", width);
	Yosys::RTLIL::Wire *y = m->addWire("\\y", width+1);
	a->port_input = true;
	b->port_input = true;
	y->port_output = true;

	m->fixup_ports();

	m->addAdd(NEW_ID, a, b, y);

	return m;
}


int main() {
	Yosys::logger().add_sink<Yosys::ConsoleLogSink>();
	Yosys::yosys_setup();

	std::string dirname = Yosys::yosys_share_dirname;
	std::cout << "Yosys share directory: " << dirname << std::endl;
	Yosys::yosys_share_dirname = YOSYS_DATADIR "/";
	std::cout << "Updated Yosys share directory: " << Yosys::yosys_share_dirname << std::endl;
	Yosys::yosys_abc_executable = YOSYS_ABC_EXECUTABLE;

	Yosys::RTLIL::Design *d = Yosys::yosys_get_design();
	Yosys::RTLIL::Module *m = adder(d, 10);

	Yosys::run_pass("hierarchy -top adder");
	Yosys::run_pass("techmap");
	Yosys::run_pass("abc -g AND,OR,XOR");
	Yosys::run_pass("opt_clean");

	// Count number of cells
	std::map<std::string, int> cells;
	for (Yosys::RTLIL::Cell *c : m->cells())
		cells[c->type.unescape()]++;

	// Sort by count (type as tie breaker
	std::vector<std::pair<std::string, int>> sorted_cells(cells.begin(), cells.end());
	std::sort(sorted_cells.begin(), sorted_cells.end(), [](const auto &a, const auto &b) {
		if (a.second != b.second)
			return a.second > b.second;
		return a.first < b.first;
	});

	std::cout << "\nCells in module '" << m->name.unescape() << "':" << std::endl;
	for (const auto &[type, count] : sorted_cells)
		std::cout << "\t" << count << " - " << type << std::endl;

	Yosys::yosys_shutdown();

	return 0;
}
