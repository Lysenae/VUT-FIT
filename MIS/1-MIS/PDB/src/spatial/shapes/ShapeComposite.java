// xstane33, Jiří Staněk
// xskota07, Jiří Skoták
// xklima22, Daniel Klimaj

package spatial.shapes;

import enums.ShapeState;
import enums.ShapeType;
import java.awt.Color;
import java.awt.Graphics2D;
import java.util.ArrayList;
import java.awt.Point;
import java.awt.Shape;
import oracle.spatial.geometry.JGeometry;

/**
 * Composite shape, is created by union of polygons, rectangles, circles or
 * other composite objects.
 */
public class ShapeComposite extends ShapeBase
{
    private JGeometry geometry;
    private Shape     shape;
    
    /**
     * Class constructor
     * @param id shape id
     * @param name shape name
     * @param geometry JGeometry
     */
    public ShapeComposite(int id, String name, JGeometry geometry)
    {
        super(id, name, 0, 0);
        this.geometry = geometry;
        this.shape    = this.geometry.createShape();
        this.type     = ShapeType.COMPOSITE;
        this.setState(ShapeState.OK); // Can only be loaded from DB
    }
    
    /**
     * Checks if contains point.
     * @param p point
     * @return true if contains point
     */
    @Override
    public boolean contains(Point p)
    {
        return this.shape.contains(p);
    }

    /**
     * Does nothing.
     * @param p point
     */
    @Override
    public void moveTo(Point p)
    {
        return;
    }

    /**
     * Draws shape.
     * @param g graphic context
     * @param sketch not used for composite shapes
     */
    @Override
    public void draw(Graphics2D g, boolean sketch)
    {
        g.setPaint(this.getColorForState(false));
        g.fill(this.shape);
        g.setPaint(this.getColorForState(true));
        g.draw(this.shape);
    }
    
    /**
     * Converts shape to JGeometry.
     * @return JGeometry
     */
    @Override
    public JGeometry toJGeometry()
    {
        return this.geometry;
    }
}
