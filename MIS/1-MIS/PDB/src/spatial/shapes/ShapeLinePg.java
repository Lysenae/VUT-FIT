// xstane33, Jiří Staněk
// xskota07, Jiří Skoták
// xklima22, Daniel Klimaj

package spatial.shapes;

import enums.ShapeState;
import enums.ShapeType;
import java.awt.Graphics2D;
import java.awt.Point;
import java.awt.geom.Point2D;
import java.util.ArrayList;
import oracle.spatial.geometry.JGeometry;

/**
 * Polygon. For most methods see ShapeLine.
 */
public class ShapeLinePg extends ShapeLine
{
    /**
     * Class construcotr
     * @param id shape id
     * @param name shape name
     * @param x x coordinate of first point
     * @param y y coordinate of first point
     */
    public ShapeLinePg(int id, String name, int x, int y)
    {
        super(id, x, y);
        this.setName(name);
        this.type = ShapeType.POLYGON;
        this.setOwnable();
    }
    
    /**
     * Checks if is drawable. Polygon must contain at least 3 control points.
     * @return true if polygon is drawable
     */
    @Override
    public boolean isDrawable()
    {
        return this.getPoints().size() >= 3;
    }
    
    /**
     * Draw polygon.
     * @param g graphic context
     * @param sketch determines if polygon is being sketched
     */
    @Override
    public void draw(Graphics2D g, boolean sketch)
    {
        if(sketch)
        {
            super.draw(g, sketch);
        }
        else
        {
            // Inner
            g.setColor(this.getColorForState(false));
            g.fillPolygon(this.xPts(), this.yPts(), this.getPoints().size());

            // Outline
            g.setColor(this.getColorForState(true));
            if(sketch) g.setStroke(ShapeBase.DASH);
            Point p1, p2;
            int max = this.getPoints().size()-1;
            int o = ShapeLine.PT_OFFSET;
            int s = o*2+1;

            if(this.getPoints().size() == 1)
            {
                p1 = this.getPoints().get(0);
                g.fillOval(p1.x-o, p1.y-o, s, s);
            }

            for(int pt=0; pt<max; ++pt)
            {
                p1 = this.getPoints().get(pt);
                p2 = this.getPoints().get(pt+1);
                g.drawLine(p1.x, p1.y, p2.x, p2.y);

                g.fillOval(p1.x-o, p1.y-o, s, s);
                if(pt == max-1)
                {
                    p1 = this.getPoints().get(0);
                    g.fillOval(p2.x-o, p2.y-o, s, s);
                    g.drawLine(p1.x, p1.y, p2.x, p2.y);
                }
            }
        }
    }
    
    /**
     * Gets list of x coordinates of control points.
     * @return x coordinates
     */
    private int[] xPts()
    {
        int size = this.getPoints().size();
        int[] pts = new int[size];
        for(int i=0; i<size; ++i)
        {
            pts[i] = this.getPoints().get(i).x;
        }
        return pts;    
    }
    
    /**
     * Gets list of y coordinates of control points.
     * @return y coordinates
     */
    private int[] yPts()
    {
        int size = this.getPoints().size();
        int[] pts = new int[size];
        for(int i=0; i<size; ++i)
        {
            pts[i] = this.getPoints().get(i).y;
        }
        return pts;    
    }
    
    /**
     * Gets array of points.
     * @return array of points
     */
    private double[] pts()
    {
        int size = this.getPoints().size();
        double[] p = new double[2*size];
        int[] x = this.xPts();
        int[] y = this.yPts();
        for(int i=0; i<size; ++i)
        {
            p[i*2]   = x[i];
            p[i*2+1] = y[i];
        }
        return p;
    }
    
    /**
     * Create new polygon from JGeometry.
     * @param id shape id
     * @param name shape name
     * @param geo JGeometry
     * @return polygon
     */
    public static ShapeLinePg fromJGeometry(int id, String name, JGeometry geo)
    {
        double[] pts = geo.getOrdinatesArray();
        System.out.println(pts.length);
        ShapeLinePg lpg = new ShapeLinePg(id, name, (int)pts[0], (int)pts[1]);
        for(int i=2; i<pts.length; i+=2)
            lpg.addPoint((int)pts[i], (int)pts[i+1]);
        lpg.setState(ShapeState.OK);
        return lpg;
    }
    
    /**
     * Converts polygon to JGeometry.
     * @return JGeometry
     */
    public JGeometry toJGeometry()
    {
        return JGeometry.createLinearPolygon(this.pts(), 2, 0);
    }
}
