import random

# Define lists of possible features
eyes = ['😀', '😬', '😂', '😄', '😅', '😇', '😉', '😊', '😋', '😌', '😍', '😘', '😗', '😙', '😚', '😜', '😝', '😛', '😎', '😏']
noses = ['👃', '🐽', '👃', '🐽', '👃']
mouths = ['😁', '😆', '😅', '😂', '😊', '😇', '😉', '😍', '😘', '😗', '😋', '😛', '😝', '😜', '😚', '😐', '😶', '😏', '😣', '😥', '😮', '😯', '😪', '😫', '😴', '😌', '😛', '😜', '😝', '😒', '😓', '😔', '😕', '😖', '😷', '😲', '😞', '😟', '😤', '😢', '😭', '😦', '😧', '😨', '😬', '😰', '😱', '😳', '😵', '😡', '😠']

# Function to generate a random face
def generate_face():
    return f"{random.choice(eyes)}{random.choice(noses)}{random.choice(mouths)}"

# Generate and print 5 random faces
for _ in range(5):
    print(generate_face())
