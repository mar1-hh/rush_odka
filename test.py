import random

animals = [f"animal{i}" for i in range(1000000)]
sounds = [f"sound{i}" for i in range(1000000)]

dictionary_section = "\n".join(f"{animal}\n{sound}" for animal, sound in zip(animals, sounds))

extra_animals = animals + [f"unknown{i}" for i in range(1000000)]
search_animals = [random.choice(extra_animals) for _ in range(1000000)]
search_section = "\n".join(search_animals)

hotrace_file = f"{dictionary_section}\n\n{search_section}\n"

with open("hotrace.txt", "w") as f:
    f.write(hotrace_file)

print("hotrace.txt created !! exec : ./hotrace < hotrace.txt")