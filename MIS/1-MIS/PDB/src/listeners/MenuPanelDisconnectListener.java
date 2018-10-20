
package listeners;
import gui.MainWindow;
import gui.panels.MenuPanel;
import enums.Panel;
import models.Database;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.sql.SQLException;
import javax.swing.JPanel;

/**
 * @author xstane33
 * @author xskota07
 * @author xklima22
 */
public class MenuPanelDisconnectListener implements ActionListener {

    private final MenuPanel menuPanel;

    public MenuPanelDisconnectListener(MenuPanel menuPanel) {
        this.menuPanel = menuPanel;
    }

    @Override
    public void actionPerformed(ActionEvent e) {
        try {
            // Try disconnect and set disconnected layout.
            Database.sharedInstance().disconnect();
            this.menuPanel.setButtonsToNotConnectState();
            MainWindow.getMainWindow().setWorkspace(new JPanel(), Panel.LOGIN);
            
            //this.menuBar.setConnected(false);
        }
        catch (SQLException ex) {
            System.out.println("Disconnect error");
        }
    }

}
