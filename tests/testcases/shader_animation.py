#!/usr/bin/env python3

# E2E test for the Burn-My-Windows shader animations: verifies that a
# window open/close animation driven by a GLSL shader actually renders
# (pixels change while the animation runs, and stop changing after it
# finishes). Runs only under a shader-capable backend; skipped
# otherwise. See configs/shader_animation.conf.

import sys
import time

import xcffib
import xcffib.xproto as xproto

from common import set_window_name, set_window_class

LOG_PATH = "log"
SKIP_MARKER = "Shaders are not supported by selected backend"
READY_MARKER = "Loading shader source from bmw-fire.frag"

WIN_X, WIN_Y, WIN_W, WIN_H = 10, 10, 200, 200
# Bytes that must change between two captures while the animation runs.
ANIMATING_MIN_DIFF = 1000
# Bytes allowed to change between two captures after the animation
# finished (the window content itself is static).
SETTLED_MAX_DIFF = 10

ANIMATION_DURATION = 1.0


def wait_for_shader_backend():
    """Wait until the picom log shows the backend supports shaders.

    Exits successfully when the backend cannot run shader animations
    (dummy, xrender, ...); raises when no marker appears at all.
    """
    deadline = time.time() + 10.0
    while time.time() < deadline:
        try:
            with open(LOG_PATH) as f:
                content = f.read()
        except FileNotFoundError:
            content = ""
        if SKIP_MARKER in content:
            print("Backend does not support shaders, skipping")
            sys.exit(0)
        if READY_MARKER in content:
            return
        time.sleep(0.2)
    raise RuntimeError("picom log contains neither the skip nor the "
                       "ready marker")


def fill_window(conn, wid):
    """Fill the window with a solid, distinctive color."""
    gc = conn.generate_id()
    conn.core.CreateGCChecked(
        gc, wid,
        xproto.GC.Foreground | xproto.GC.GraphicsExposures,
        [0x404040, 0]).check()
    rect = xproto.RECTANGLE.synthetic(0, 0, WIN_W, WIN_H)
    conn.core.PolyFillRectangleChecked(wid, gc, 1, [rect]).check()


def capture(conn, root):
    """Return the pixel bytes of the window region of the screen."""
    reply = conn.core.GetImage(
        xproto.ImageFormat.ZPixmap, root, WIN_X, WIN_Y, WIN_W, WIN_H,
        0xffffffff).reply()
    return bytes(reply.data.buf())


def diff_count(a, b):
    assert len(a) == len(b)
    return sum(1 for x, y in zip(a, b) if x != y)


def main():
    wait_for_shader_backend()
    # Give picom a moment to finish initializing after the marker.
    time.sleep(0.5)

    conn = xcffib.connect()
    setup = conn.get_setup()
    root = setup.roots[0].root
    visual = setup.roots[0].root_visual
    depth = setup.roots[0].root_depth

    wid = conn.generate_id()
    conn.core.CreateWindowChecked(
        depth, wid, root, WIN_X, WIN_Y, WIN_W, WIN_H, 0,
        xproto.WindowClass.InputOutput, visual, 0, []).check()
    set_window_name(conn, wid, "ShaderAnimationTest")
    set_window_class(conn, wid, "ShaderAnimationTest")

    # Map the window: the open animation should start and animate the
    # window's pixels.
    conn.core.MapWindowChecked(wid).check()
    fill_window(conn, wid)
    time.sleep(0.4)

    a = capture(conn, root)
    time.sleep(0.3)
    b = capture(conn, root)
    print("open animating diff:", diff_count(a, b))
    assert diff_count(a, b) > ANIMATING_MIN_DIFF, \
        "open animation is not rendering any changing pixels"

    # Once the animation finished the window content is static, so the
    # pixels must stop changing.
    time.sleep(ANIMATION_DURATION * 2)
    c = capture(conn, root)
    time.sleep(0.3)
    d = capture(conn, root)
    print("open settled diff:", diff_count(c, d))
    assert diff_count(c, d) <= SETTLED_MAX_DIFF, \
        "pixels keep changing after the open animation ended"

    # Destroy the window: the close animation should keep animating the
    # pixels. If the close animation is missing entirely, the window just
    # disappears and both captures show the same static background.
    conn.core.DestroyWindowChecked(wid).check()
    e = capture(conn, root)
    time.sleep(0.3)
    f = capture(conn, root)
    print("close animating diff:", diff_count(e, f))
    assert diff_count(e, f) > ANIMATING_MIN_DIFF, \
        "close animation is not rendering any changing pixels"

    # After the close animation everything is settled background.
    time.sleep(ANIMATION_DURATION * 2)
    g = capture(conn, root)
    time.sleep(0.3)
    h = capture(conn, root)
    print("close settled diff:", diff_count(g, h))
    assert diff_count(g, h) <= SETTLED_MAX_DIFF, \
        "pixels keep changing after the close animation ended"

    print("shader animation e2e OK")


if __name__ == "__main__":
    main()
