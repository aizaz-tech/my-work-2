def get_float(prompt):
    while True:
        try:
            return float(input(prompt))
        except ValueError:
            print("Invalid input. Please enter a number.")

def show_results(name, a, b):
    print(f"\nHi {name}! You entered: {a:.1f} and {b:.1f}")

    print(f"Sum: {a + b:.2f}")
    print(f"Difference: {a - b:.2f}")
    print(f"Product: {a * b:.2f}")
    
    if b != 0:
        print(f"Division: {a / b:.2f}")
    else:
        print("Division: undefined (cannot divide by zero)")

def main():
    print("Welcome to the ProSensia Calculator!\n")

    name = input("Enter your name: ").strip()
    if not name:
        name = "User"

    print("\nPlease enter two numbers:")
    num1 = get_float("Enter first number: ")
    num2 = get_float("Enter second number: ")

    show_results(name, num1, num2)

if __name__ == "__main__":
    main()












    
