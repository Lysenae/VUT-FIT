// xstane33, Jiří Staněk
// xskota07, Jiří Skoták
// xklima22, Daniel Klimaj

package showground;

import gui.MainWindow;
import java.awt.Dimension;


/**
 *
 * @author xstane33
 */
public class Application {
    private final MainWindow window;

    /**
     * Class constructor
     */
    public Application(){
        this.window = new MainWindow();
        this.window.setSize(new Dimension(800, 600));
        this.window.setMinimumSize(new Dimension(800, 500));
        this.window.setLocationRelativeTo(null);
        this.window.setVisible(true);
        
    
    }
    /**
     * Application main
     * @param args arguments
     */
    public static void main(String[] args) {
        javax.swing.SwingUtilities.invokeLater(new Runnable() {
            @Override
            public void run() {
                Application app = new Application();
            }
        });
    }
}
