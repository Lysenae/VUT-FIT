

package listeners;
import gui.panels.MenuPanel;
import gui.MainWindow;
import gui.panels.LayoutManagerPanel;
import enums.Panel;

import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;

/**
 * Show layout manager
 * @author xstane33
 * @author xskota07
 * @author xklima22
 */
public class MenuPanelLayoutMgrListener implements ActionListener
{
    /**
     * Class constructor.
     * @param menuPanel parent
     */
    public MenuPanelLayoutMgrListener(MenuPanel menuPanel)
    {
    }
    
    /**
     * Button clicked.
     * @param e event data
     */
    @Override
    public void actionPerformed(ActionEvent e)
    {
        LayoutManagerPanel ws = new LayoutManagerPanel();
        MainWindow.getMainWindow().setWorkspace(ws, Panel.LAYOUT_MANAGER);
    }
}


