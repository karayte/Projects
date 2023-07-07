import time
from pysat.formula import CNF
from pysat.solvers import Glucose4

start_time = time.time()


def find_empty_location(board_, location):
    for row_ in range(9):
        for col in range(9):
            if board_[row_][col] == 0:
                location[0] = row_
                location[1] = col
                return True
    return False


def is_in_row(board_, row_, num):
    return any(num == board_[row_][col] for col in range(9))


def is_in_column(board_, col, num):
    return any(num == board_[row_][col] for row_ in range(9))


def is_in_box(board_, box_start_row, box_start_col, num):
    for row_ in range(3):
        for col in range(3):
            if board_[row_ + box_start_row][col + box_start_col] == num:
                return True
    return False


def is_safe(board_, row_, col, num):
    return not is_in_row(board_, row_, num) and not is_in_column(board_, col, num) \
        and not is_in_box(board_, row_ - row_ % 3, col - col % 3, num)


def solve_sudoku_brute_force(board_):
    location = [0, 0]

    if not find_empty_location(board_, location):
        return True

    row_ = location[0]
    col = location[1]

    for num in range(1, 10):
        if is_safe(board_, row_, col, num):
            board_[row_][col] = num
            if solve_sudoku_brute_force(board_):
                return True
            board_[row_][col] = 0
    return False


class CSP:
    def __init__(self, variables, domains, constraints):
        self.variables = variables
        self.domains = domains
        self.constraints = constraints

    def is_satisfied(self, var, value, assignment):
        for neighbor in self.constraints[var]:
            if neighbor in assignment:
                neighbor_value = assignment[neighbor]
                if neighbor_value != 0 and neighbor_value == value:
                    return False
        return True

    @staticmethod
    def assign(var, value, assignment):
        assignment[var] = value

    @staticmethod
    def revert_assignment(var, assignment):
        if var in assignment:
            del assignment[var]

    def n_conflicts(self, var, value, assignment):
        def conflict(var2):
            return var2 in assignment and not assignment[var2] != value
        return sum(conflict(v) for v in self.constraints[var])


def backtracking_search(csp):
    return backtrack({}, csp)


def backtrack(assignment, csp):
    if len(assignment) == len(csp.variables):
        return assignment
    var = select_unassigned_variable(assignment, csp)
    for value in csp.domains[var]:
        if csp.is_satisfied(var, value, assignment):
            csp.assign(var, value, assignment)
            result = backtrack(assignment, csp)
            if result is not None:
                return result
        csp.revert_assignment(var, assignment)
    return None


# Implementing MRV Heuristic
def select_unassigned_variable(assignment, csp):
    unassigned_vars = [v for v in csp.variables if v not in assignment]
    return min(unassigned_vars, key=lambda var: len(csp.domains[var]))


def convert_sudoku_board_to_csp(sudoku):
    variables = [(i, j) for i in range(9) for j in range(9) if sudoku[i][j] == 0]
    domains = {(i, j): list(range(1, 10)) for i in range(9) for j in range(9)}

    for i in range(9):
        for j in range(9):
            if sudoku[i][j] != 0:
                value = sudoku[i][j]
                for k in range(9):
                    if (k, j) in domains and value in domains[(k, j)]:
                        domains[(k, j)].remove(value)
                    if (i, k) in domains and value in domains[(i, k)]:
                        domains[(i, k)].remove(value)

                start_row = (i // 3) * 3
                start_col = (j // 3) * 3
                for m in range(start_row, start_row + 3):
                    for n in range(start_col, start_col + 3):
                        if (m, n) in domains and value in domains[(m, n)]:
                            domains[(m, n)].remove(value)

    constraints = {(i, j): set() for i in range(9) for j in range(9)}
    for i in range(9):
        for j in range(9):
            for k in range(9):
                if i != k:
                    constraints[(i, j)].add((k, j))
                if j != k:
                    constraints[(i, j)].add((i, k))
            for k in range((i // 3) * 3, (i // 3) * 3 + 3):
                for location in range((j // 3) * 3, (j // 3) * 3 + 3):
                    if i != k or j != location:
                        constraints[(i, j)].add((k, location))

    return CSP(variables, domains, constraints)


def solve_sudoku_CSP(sudoku):
    csp = convert_sudoku_board_to_csp(sudoku)
    assignment = backtracking_search(csp)
    for var, value in assignment.items():
        sudoku[var[0]][var[1]] = value
    return sudoku


def variables_to_numbers(i, j, d):
    return 81 * (i - 1) + 9 * (j - 1) + d


def set_sudoku_rules():
    block = [[(i, j) for i in range(3 * x + 1, 3 * x + 4) for j in range(3 * y + 1, 3 * y + 4)] for x in range(3) for y
             in range(3)]
    line_ = [[(i, j) for j in range(1, 10)] for i in range(1, 10)]
    col = [[(i, j) for i in range(1, 10)] for j in range(1, 10)]

    rules = []
    for ind in block + line_ + col:
        for x in range(9):
            for y in range(x + 1, 9):
                i, j = ind[x]
                k, location = ind[y]
                for d in range(1, 10):
                    rules.append([-variables_to_numbers(i, j, d), -variables_to_numbers(k, location, d)])

    # at least one number in each entry
    for i in range(1, 10):
        for j in range(1, 10):
            rules.append([variables_to_numbers(i, j, d) for d in range(1, 10)])

    return rules


def initial_board_clauses(board_):
    clauses = []
    for i in range(9):
        for j in range(9):
            d = board_[i][j]
            if d != 0:
                clauses.append([variables_to_numbers(i + 1, j + 1, d)])
    return clauses


def board_to_cnf(board_):
    clauses = set_sudoku_rules() + initial_board_clauses(board_)
    return CNF(from_clauses=clauses)


def solve_sudoku_SAT(board_):
    cnf = board_to_cnf(board_)
    solver = Glucose4()
    solver.append_formula(cnf.clauses)
    if not solver.solve():
        return None
    model = solver.get_model()
    solution_ = [[0 for _ in range(9)] for _ in range(9)]
    for var in model:
        if var > 0:
            d = (var - 1) % 9 + 1
            j = ((var - 1) // 9) % 9 + 1
            i = ((var - 1) // 81) + 1
            solution_[i - 1][j - 1] = d
    return solution_


def is_valid_sudoku(board_):
    for i in range(9):
        if not is_valid_unit([board_[i][j] for j in range(9)]) or \
           not is_valid_unit([board_[j][i] for j in range(9)]):
            return False
    for i in range(0, 9, 3):
        for j in range(0, 9, 3):
            if not is_valid_unit([board_[x][y] for x in range(i, i + 3) for y in range(j, j + 3)]):
                return False
    return True


def is_valid_unit(unit):
    unit = [i for i in unit if i != 0]
    return len(set(unit)) == len(unit)


def start_CSP_and_print_results(board_):
    solution = solve_sudoku_CSP(board_)

    for row_ in solution:
        print(row_)


def start_SAT_and_print_results(board_):
    solution = solve_sudoku_SAT(board_)

    for row_ in solution:
        print(row_)


def start_brute_force_and_print(board_):
    if solve_sudoku_brute_force(board_):
        for i_ in range(9):
            for j_ in range(9):
                print(board_[i_][j_], end=" ")
            print()


if __name__ == "__main__":

    board = []

    with open('boardMedium.txt', 'r') as file:
        for line in file:
            row = [int(num) for num in line.split()]
            board.append(row)

    if not is_valid_sudoku(board):
        print("Unfortunately, given Sudoku board is invalid. Please, give me another one and rerun the program.\n")
    else:
        algorithm_name = input(
            'Choose one algorithm to run in three options: 1 - brute force, 2 - CSP, any other number - SAT.\n')
        if algorithm_name == 1:
            # brute force solution
            start_brute_force_and_print(board)
        elif algorithm_name == 2:
            # CSP algorithm
            start_CSP_and_print_results(board)
        else:
            # SAT algorithm
            start_SAT_and_print_results(board)
