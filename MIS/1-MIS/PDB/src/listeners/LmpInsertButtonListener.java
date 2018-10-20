package listeners;

import enums.LmMode;
import gui.dialogs.NewReservationDialog;
import gui.panels.LayoutManagerPanel;
import helpers.DateHelper;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.util.Date;
import javax.swing.JOptionPane;

/**
 * LayoutManagerPanel Create reservation button clicked listener.
 * @author xstane33
 * @author xskota07
 * @author xklima22
 */
public class LmpInsertButtonListener implements ActionListener
{
    private final LayoutManagerPanel parent;
    
    /**
     * Class constructor.
     * @param parent parent panel
     */
    public LmpInsertButtonListener(LayoutManagerPanel parent)
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
        NewReservationDialog newReservationDialog = new NewReservationDialog(
            this.parent.getMapPanel());
        this.parent.unselectedAll();
        Object[] options = {"OK", "Cancel"};

        int i = JOptionPane.showOptionDialog(
            null, newReservationDialog, "Add New Reservation",
            JOptionPane.PLAIN_MESSAGE, JOptionPane.PLAIN_MESSAGE, null,
            options, options[0]);

        if (i == 0)
        {
            LmMode selectedMode = LmMode.INSERT;

            if (newReservationDialog.aliasTextField.getText().isEmpty())
            {
                JOptionPane.showMessageDialog(null,
                    "You forgot reservation alias. No reservation was created.",
                    "Warning", JOptionPane.WARNING_MESSAGE);
                return;
            }

            if (newReservationDialog.toDate.getDate().compareTo(
            this.parent.getDate()) < 0)
            {
                JOptionPane.showMessageDialog(null,
                        "Dates are not consequent. No reservation was created.",
                        "Warning", JOptionPane.WARNING_MESSAGE);
                return;
            }

            this.parent.getMapPanel().setSelectedMode(selectedMode);

            this.parent.getMapPanel().reservation.setCustomerId(
                newReservationDialog.getIdforSelectedCustomer());
            Date dateFrom = DateHelper.getStartOfDay(this.parent.getDate());
            java.sql.Date sqlDateFrom = new java.sql.Date(dateFrom.getTime());
            this.parent.getMapPanel().reservation.setFromDate(sqlDateFrom);

            Date dateTo = DateHelper.getEndOfDay(
                newReservationDialog.toDate.getDate());
            java.sql.Date sqlDateTo = new java.sql.Date(dateTo.getTime());
            this.parent.getMapPanel().reservation.setToDate(sqlDateTo);
            this.parent.getMapPanel().setNewShapeName(
                newReservationDialog.aliasTextField.getText());
        } 
    }
}
