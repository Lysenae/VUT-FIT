

package listeners;

import gui.panels.LayoutManagerPanel;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;

/**
 * LayoutManagerPanel Show reservation for given date button clicked listener.
 * @author xstane33
 * @author xskota07
 * @author xklima22
 */
public class LmpShowButtonListener implements ActionListener
{
    private final LayoutManagerPanel parent;
    
    /**
     * Class constructor.
     * @param parent parent panel
     */
    public LmpShowButtonListener(LayoutManagerPanel parent)
    {
        this.parent = parent;
    }

    /**
     * Button clicked event handler.
     * @param e event data
     */
    @Override
    public void actionPerformed(ActionEvent e)
    {
        this.parent.getMapPanel().loadAll();
    }
}
