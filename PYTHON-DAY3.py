def add_student(record_set, name, grade):
    if any(student[0].lower() == name.lower() for student in record_set):
        return (False, f"Error: Student '{name}' already exists.", record_set)
    
    try:
        grade = float(grade)
    except ValueError:
        return (False, "Error: Grade must be a number.", record_set)
    
    if grade < 0 or grade > 100:
        return (False, "Error: Grade must be between 0 and 100.", record_set)
    
    new_record = (name.strip(), grade)
    updated_set = record_set.copy()
    updated_set.add(new_record)
    return (True, f"Student '{name}' added successfully.", updated_set)

def view_all_students(record_set):
    return sorted(record_set, key=lambda x: x[0].lower())

def get_top_student(record_set):
    if not record_set:
        return None
    return max(record_set, key=lambda x: x[1])

def filter_students(record_set, min_grade):
    try:
        min_grade = float(min_grade)
    except ValueError:
        return []
    
    filtered = [student for student in record_set if student[1] >= min_grade]
    return sorted(filtered, key=lambda x: x[0].lower())

def calculate_statistics(record_set):
    if not record_set:
        return {
            'average': 0,
            'above_average_count': 0,
            'total_students': 0,
            'above_average_students': []
        }
    
    grades = [student[1] for student in record_set]
    average = sum(grades) / len(grades)
    above_avg = [student for student in record_set if student[1] > average]
    
    return {
        'average': round(average, 2),
        'above_average_count': len(above_avg),
        'total_students': len(record_set),
        'above_average_students': sorted(above_avg, key=lambda x: x[0].lower())
    }

def display_menu():
    print("\nStudent Record Manager Menu:")
    print("1. Add a new student")
    print("2. View all students")
    print("3. View top student")
    print("4. Filter students by minimum grade")
    print("5. View class statistics")
    print("6. Exit")

def validate_input(input_str):
    parts = [part.strip() for part in input_str.split(',')]
    if len(parts) != 2:
        return None
    name, grade = parts
    if not name or not grade:
        return None
    return (name, grade)

def main():
    student_records = set()
    
    print("Welcome to Student Record Manager!")
    print("Enter student records in format: 'Name, Grade'")
    print("Type 'menu' to access other options or 'exit' to quit")
    
    while True:
        user_input = input("\nEnter student record or command: ").strip()
        
        if user_input.lower() == 'exit':
            break
            
        if user_input.lower() == 'menu':
            while True:
                display_menu()
                choice = input("Enter your choice (1-6): ").strip()
                
                if choice == '1':
                    record_input = input("Enter student (Name, Grade): ").strip()
                    validated = validate_input(record_input)
                    if not validated:
                        print("Invalid input format. Use 'Name, Grade'")
                        continue
                    name, grade = validated
                    success, message, student_records = add_student(student_records, name, grade)
                    print(message)
                    
                elif choice == '2':
                    students = view_all_students(student_records)
                    if not students:
                        print("No student records available.")
                    else:
                        print("\nAll Students:")
                        for student in students:
                            print(f"{student[0]}: {student[1]}")
                            
                elif choice == '3':
                    top_student = get_top_student(student_records)
                    if top_student:
                        print(f"\nTop Student: {top_student[0]} with grade {top_student[1]}")
                    else:
                        print("No student records available.")
                        
                elif choice == '4':
                    min_grade = input("Enter minimum grade: ").strip()
                    filtered = filter_students(student_records, min_grade)
                    if not filtered:
                        print("No students meet this grade threshold or invalid input.")
                    else:
                        print(f"\nStudents with grade ≥ {min_grade}:")
                        for student in filtered:
                            print(f"{student[0]}: {student[1]}")
                            
                elif choice == '5':
                    stats = calculate_statistics(student_records)
                    print("\nClass Statistics:")
                    print(f"Total students: {stats['total_students']}")
                    print(f"Average grade: {stats['average']}")
                    print(f"Students above average: {stats['above_average_count']}")
                    if stats['above_average_students']:
                        print("Above average students:")
                        for student in stats['above_average_students']:
                            print(f"- {student[0]}: {student[1]}")
                            
                elif choice == '6':
                    print("Returning to main input...")
                    break
                    
                else:
                    print("Invalid choice. Please enter 1-6.")
            continue
            
        validated = validate_input(user_input)
        if not validated:
            print("Invalid input format. Use 'Name, Grade' or type 'menu' for options")
            continue
            
        name, grade = validated
        success, message, student_records = add_student(student_records, name, grade)
        print(message)
    
    print("\nFinal Class Report:")
    if not student_records:
        print("No student records entered.")
    else:
        stats = calculate_statistics(student_records)
        students = view_all_students(student_records)
        
        print(f"\nTotal Students: {stats['total_students']}")
        print(f"Average Grade: {stats['average']}")
        
        top_student = get_top_student(student_records)
        print(f"\nTop Student: {top_student[0]} - {top_student[1]}")
        
        print("\nAll Students (alphabetical order):")
        for student in students:
            print(f"{student[0]}: {student[1]}")
        
        print("\nStudents Above Average:")
        for student in stats['above_average_students']:
            print(f"{student[0]}: {student[1]}")

if __name__ == "__main__":
    main()