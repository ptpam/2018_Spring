import numpy as np
import matplotlib.pyplot as plt

w = 1000
h = 1000

def normalize(x):
    x /= np.linalg.norm(x)
    return x

def intersect_plane(O, D, P, N):
    # Return the distance from O to the intersection of the ray (O, D) with the
    # plane (P, N), or +inf if there is no intersection.
    # O and P are 3D points, D and N (normal) are normalized vectors.
    denom = np.dot(D, N)
    if np.abs(denom) < 1e-6:
        return np.inf
    d = np.dot(P - O, N) / denom
    if d < 0:
        return np.inf
    return d

def intersect_sphere(O, D, S, R):
    # Return the distance from O to the intersection of the ray (O, D) with the
    # sphere (S, R), or +inf if there is no intersection.
    # O and S are 3D points, D (direction) is a normalized vector, R is a scalar.
    a = np.dot(D, D)
    OS = O - S
    b = 2 * np.dot(D, OS)
    c = np.dot(OS, OS) - R * R
    disc = b * b - 4 * a * c
    if disc > 0:
        distSqrt = np.sqrt(disc)
        q = (-b - distSqrt) / 2.0 if b < 0 else (-b + distSqrt) / 2.0
        t0 = q / a
        t1 = c / q
        t0, t1 = min(t0, t1), max(t0, t1)
        if t1 >= 0:
            return t1 if t0 < 0 else t0
    return np.inf

def intersect(O, D, obj):
    if obj['type'] == 'plane':
        return intersect_plane(O, D, obj['position'], obj['normal'])
    elif obj['type'] == 'sphere':
        return intersect_sphere(O, D, obj['position'], obj['radius'])

def get_normal(obj, M):
    # Find normal.
    if obj['type'] == 'sphere':
        N = normalize(M - obj['position'])
    elif obj['type'] == 'plane':
        N = obj['normal']
    return N

def get_color(obj, M):
    color = obj['color']
    if not hasattr(color, '__len__'):
        color = color(M)
    return color

def trace_ray(rayO, rayD, light_list):
    # Find first point of intersection with the scene.
    t = np.inf
    for i, obj in enumerate(scene):
        t_obj = intersect(rayO, rayD, obj)
        if t_obj < t:
            t, obj_idx = t_obj, i
    # Return None if the ray does not intersect any object.
    if t == np.inf:
        return
    # Find the object.
    obj = scene[obj_idx]
    # Find the point of intersection on the object.
    M = rayO + rayD * t
    # Find properties of the object.
    N = get_normal(obj, M)
    color = get_color(obj, M)

    # Start computing the color.
    col_ray = ambient
    for L in light_list:
        check_shadow = False
        toL = normalize(L - M)
        toO = normalize(O - M)
        # Shadow: find if the point is shadowed or not.
        l = [intersect(M + N * .0001, toL, obj_sh)
                for k, obj_sh in enumerate(scene) if k != obj_idx]
        if l and min(l) < np.inf:
            col_ray += obj.get('diffuse_c', 0) * min(np.dot(N, toL), 0) * color
        else:
            # Lambert shading (diffuse).
            col_ray += obj.get('diffuse_c', diffuse_c) * max(np.dot(N, toL), 0) * color
            # Blinn-Phong shading (specular).
            col_ray += obj.get('specular_c', specular_c) * max(np.dot(N, normalize(toL + toO)), 0) ** specular_k * color_light
    return obj, M, N, col_ray/len(light_list)

def add_sphere(position, radius, color):
    return dict(type='sphere', position=np.array(position),
        radius=np.array(radius), color=np.array(color), reflection=.5)

def add_plane(position, normal):
    return dict(type='plane', position=np.array(position),
        normal=np.array(normal),
        color=lambda M: (color_plane0
            if (int(M[0] * 2) % 2) == (int(M[2] * 2) % 2) else color_plane1),
        diffuse_c=.75, specular_c=.5, reflection=.25)

# List of objects.
color_plane0 = 1. * np.ones(3)
color_plane1 = 1. * np.ones(3)

scene = []
num_spheres_str = input('Number of spheres: ')
num_spheres = int(num_spheres_str)

for x in range(0, num_spheres):
    s_color_str = input('Color "R,G,B" of sphere  #' + str(x+1) + ': ')
    c_r_str, c_g_str, c_b_str = s_color_str.split(",")
    c_r = round(float(c_r_str)/255, 3)
    c_g = round(float(c_g_str)/255, 3)
    c_b = round(float(c_b_str)/255, 3)
    s_pos = input('Position "x, y, z" of sphere #' + str(x+1) + ': ')
    s_x_str, s_y_str, s_z_str = s_pos.split(",")
    s_x = round(float(s_x_str)/50, 3)
    s_y = round(float(s_y_str)/50, 3)
    s_z = round(float(s_z_str)/50, 3)
    s_rad_str = input('Radius of sphere #' + str(x+1) + ': ')
    s_rad = round(float(s_rad_str)/50, 3)
    # List of objects.
    scene.append(add_sphere([s_x, s_y, s_z],s_rad,[c_r, c_g, c_b]))

scene.append(add_plane([0., -.999, 0.], [0., .5, 0.]))


# Light position and color.
L = np.array([10., 10., 10.])
L1 = np.array([-10., 10., -10.])

light_list = []
light_list.append(L)
light_list.append(L1)
color_light = np.ones(3)

# Default light and material parameters.
ambient = .05
diffuse_c = 1.
specular_c = 1.
specular_k = 50

depth_max = 1  # Maximum number of light reflections.
col = np.zeros(3)  # Current color.
O = np.array([0., 0., 0.])  # Camera.
Q = np.array([0., 0., 2.])  # Camera pointing to.
img = np.zeros((h, w, 3))

r = float(w) / h
# Screen coordinates: x0, y0, x1, y1.
S = (-1., -1., 1., 1.)

# Loop through all pixels.
for i, x in enumerate(np.linspace(S[0], S[2], w)):
    if (i % 10 == 0) and (i % 5 == 0):
        print(i / float(w) * 100, "%")
    for j, y in enumerate(np.linspace(S[1], S[3], h)):
        col[:] = 0
        Q[:2] = (x, y)
        D = normalize(Q - O)
        depth = 0
        rayO, rayD = O, D
        # Loop through initial and secondary rays.
        while depth < depth_max:
            traced = trace_ray(rayO, rayD, light_list)
            if not traced:
                break
            obj, M, N, col_ray = traced
            # Reflection: create a new ray.
            rayO, rayD = M + N * .0001, normalize(rayD - 2 * np.dot(rayD, N) * N)
            depth += 1
            col += col_ray
        img[h - j - 1, i, :] = np.clip(col, 0, 1)

plt.imsave('fig.png', img)
