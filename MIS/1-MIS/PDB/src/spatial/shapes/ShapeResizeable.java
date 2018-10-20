// xstane33, Jiří Staněk
// xskota07, Jiří Skoták
// xklima22, Daniel Klimaj

package spatial.shapes;

import enums.ShapeType;
import enums.ResizeSide;
import java.awt.Point;
import java.awt.Rectangle;

/**
 * Abstract super class for resizeable shapes - circles and rectangles.
 */
public abstract class ShapeResizeable extends ShapeBase
{
    protected int width;
    protected int height;
    protected boolean toResize;

    protected static final int RESIZE_OFF = 4;

    /**
     * Class constructor.
     * @param id shape id
     * @param name shape name
     * @param x position x
     * @param y position y
     */
    public ShapeResizeable(int id, String name, int x, int y)
    {
        super(id, name, x, y);
        this.toResize = false;
    }

    /**
     * Gets width.
     * @return width
     */
    public int getWidth()
    {
        return this.width;
    }

    /**
     * Sets width.
     * @param w width
     */
    public void setWidth(int w)
    {
        this.width = w;
    }

    /**
     * Gets height.
     * @return height
     */
    public int getHeight()
    {
        return this.height;
    }

    /**
     * Sets height.
     * @param h height
     */
    public void setHeight(int h)
    {
        this.height = h;
    }

    /**
     * Overrides base class' resizeable flag.
     * @return true, always
     */
    @Override
    public boolean isResizeable()
    {
        return true;
    }

    /**
     * Determines which side is being resized.
     * @param p point
     * @return side id
     */
    public ResizeSide getResizeSide(Point p)
    {
        ResizeSide rs = ResizeSide.NONE;

        if(p.x >= this.getX()-RESIZE_OFF && p.x <= this.getX()+RESIZE_OFF &&
                p.y >= this.getY() && p.y <= this.getY()+this.getHeight())
        {
            rs = ResizeSide.LEFT;
        }
        else if(p.x >= this.getX()+this.getWidth()-RESIZE_OFF &&
                p.x <= this.getX()+this.getWidth()+RESIZE_OFF &&
                p.y >= this.getY() && p.y <= this.getY()+this.getHeight())
        {
            rs = ResizeSide.RIGHT;
        }
        else if(p.x >= this.getX() && p.x <= this.getX()+this.getWidth() &&
                p.y >= this.getY()-RESIZE_OFF && p.y <= this.getY()+RESIZE_OFF)
        {
            rs = ResizeSide.TOP;
        }
        else if(p.x >= this.getX() && p.x <= this.getX()+this.getWidth() &&
                p.y >= this.getY()+this.getHeight()-RESIZE_OFF &&
                p.y <= this.getY()+this.getHeight()+RESIZE_OFF)
        {
            rs = ResizeSide.BOTTOM;
        }

        return rs;
    }

    /**
     * Checks if shape is being resized at the moment.
     * @return true if shape is being resized
     */
    public boolean isResized()
    {
        return this.toResize;
    }

    /**
     * Sets resized flag to value.
     * @param r value
     */
    public void setResized(boolean r)
    {
        this.toResize = r;
    }

    public abstract void setGeometry(Point p1, Point p2, boolean enlarge);
    public abstract void setGeometry(Rectangle r, boolean enlarge);
    public abstract void resize(ResizeSide side, Point p, Point origin);
}
