package helpers;

import enums.ShapeType;

/**
 * Stores currently selected shape type.
 * @author xskota07
 * @author xklima22
 */
public class ShapeTypeSelector
{
    private static ShapeType current = ShapeType.NONE;
    
    /**
     * Gets selected shape type.
     * @return selected shape type
     */
    public static ShapeType getType()
    {
        return ShapeTypeSelector.current;
    }
    
    /**
     * Sets current shape type.
     * @param type shape type
     */
    public static void setType(ShapeType type)
    {
        System.out.format("ShapeTypeSelector: setType: %s\n", type.name());
        ShapeTypeSelector.current = type;
    }
    
    /**
     * Checks if selected shape is point.
     * @return true if selected shape is point
     */
    public static boolean isPoint()
    {
        return ShapeTypeSelector.current == ShapeType.POINT;
    }
    
    /**
     * Checks if selected shape is line.
     * @return true if selected shape is line
     */
    public static boolean isLine()
    {
        return ShapeTypeSelector.current == ShapeType.LINE;
    }
    
    /**
     * Checks if selected shape is rectangle.
     * @return true if selected shape is rectangle
     */
    public static boolean isRect()
    {
        return ShapeTypeSelector.current == ShapeType.RECTANGLE;
    }
    
    /**
     * Checks if selected shape is circle.
     * @return true if selected shape is circle
     */
    public static boolean isCircle()
    {
        return ShapeTypeSelector.current == ShapeType.CIRCLE;
    }
    
    /**
     * Checks if selected shape is polygon.
     * @return true if selected shape is polygon
     */
    public static boolean isPolygon()
    {
        return ShapeTypeSelector.current == ShapeType.POLYGON;
    }
    
    /**
     * Checks if selected shape is composite.
     * @return true if selected shape is composite
     */
    public static boolean isComposite()
    {
        return ShapeTypeSelector.current == ShapeType.COMPOSITE;
    }
    
    /**
     * Checks if selected shape is lime or polygon.
     * @return true if selected shape is line or polygon
     */
    public static boolean isMultipoint()
    {
        return ShapeTypeSelector.current == ShapeType.POINT ||
            ShapeTypeSelector.current == ShapeType.LINE ||
            ShapeTypeSelector.current == ShapeType.POLYGON;
    }
}
