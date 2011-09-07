"""
Copyright 2007, 2008, 2009 Free Software Foundation, Inc.
This file is part of GNU Radio

GNU Radio Companion is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

GNU Radio Companion is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA
"""

import pygtk
pygtk.require('2.0')
import gtk

from Dialogs import TextDisplay
from Constants import MIN_DIALOG_WIDTH, MIN_DIALOG_HEIGHT

def get_title_label(title):
    """
    Get a title label for the params window.
    The title will be bold, underlined, and left justified.
    @param title the text of the title
    @return a gtk object
    """
    label = gtk.Label()
    label.set_markup('\n<b><span underline="low">%s</span>:</b>\n'%title)
    hbox = gtk.HBox()
    hbox.pack_start(label, False, False, padding=11)
    return hbox

class BindingsDialog(gtk.Dialog):
    """
    A dialog to set block parameters, view errors, and view documentation.
    """

    def __init__(self, fg, block):
        """
        Properties dialog contructor.
        @param block a block instance
        """
        self._hash = 0
        LABEL_SPACING = 7
        gtk.Dialog.__init__(self,
            title='Event Bindings: %s'%block.get_name(),
            buttons=(gtk.STOCK_CANCEL, gtk.RESPONSE_REJECT, gtk.STOCK_OK, gtk.RESPONSE_ACCEPT),
        )
        self._block = block
        self._fg = fg


        self.set_size_request(MIN_DIALOG_WIDTH, MIN_DIALOG_HEIGHT)
        vbox = gtk.VBox()
        #Create the scrolled window to hold all the parameters
        scrolled_window = gtk.ScrolledWindow()
        scrolled_window.set_policy(gtk.POLICY_AUTOMATIC, gtk.POLICY_AUTOMATIC)
        scrolled_window.add_with_viewport(vbox)
        self.vbox.pack_start(scrolled_window, True)
        #Params box for block parameters
        self._bindings_box = gtk.VBox()
        self._bindings_box.pack_start(get_title_label('Bindings'), False)
        self._input_object_params = list()
        #Error Messages for the block
        self._error_box = gtk.VBox()
        self._error_messages_text_display = TextDisplay()
        self._error_box.pack_start(gtk.Label(), False, False, LABEL_SPACING)
        self._error_box.pack_start(get_title_label('Error Messages'), False)
        self._error_box.pack_start(self._error_messages_text_display, False)
    

        #Add the boxes
        vbox.pack_start(self._bindings_box, False)
        vbox.pack_start(self._error_box, False)
#       vbox.pack_start(self._docs_box, False)
        #connect events
        self.connect('key-press-event', self._handle_key_press)
        self.connect('show', self._update_gui)
        #show all (performs initial gui update)
        self.show_all()


    def get_known_emitters(self):
        emitters = [];
        for b in self._fg.get_blocks():
            #emitters.extend(b._event_emit);
            #tl = map(lambda x:(b.get_id(), b._name, x), b._event_emit);
            tl = map(lambda x:(b.get_id(), b._name, x), b.events_emitted());
            emitters.extend(tl);
        return emitters;
 
   
    def get_known_handlers(self):
        handlers = [];
        for b in self._fg.get_blocks():
            tl = map(lambda x:(b.get_id(), b._name, x), b._event_handle);
            #handlers.extend(b._event_handle);
            handlers.extend(tl);
        return handlers;


    def _params_changed(self):
        """
        Have the params in this dialog changed?
        Ex: Added, removed, type change, hide change...
        To the props dialog, the hide setting of 'none' and 'part' are identical.
        Therfore, the props dialog only cares if the hide setting is/not 'all'.
        Make a hash that uniquely represents the params' state.
        @return true if changed
        """
        old_hash = self._hash
        #create a tuple of things from each param that affects the params box
        self._hash = hash(tuple([(
            hash(param), param.get_type(), param.get_hide() == 'all',
        ) for param in self._block.get_params()]))
        return self._hash != old_hash

    def _handle_changed(self, *args):
        """
        A change occured within a param:
        Rewrite/validate the block and update the gui.
        """
        #update for the block
        self.__block.rewrite()
        self._block.validate()
        self._update_gui()


    def _add_binding_impl(self, source, dest):
        # initialize parent 
        parent = None;
        
        # search for pre-existing parent of same name
        i = self.binddata.get_iter_first();
        while(i != None):
            if( str(self.binddata.get_value(i,0)) == str(source) ):
                parent = i;
                break;
            i = self.binddata.iter_next(i);
    
        # fall back to creating a new top level node
        if(parent == None):
            parent = self.binddata.append(None, [source]);

        # create child node (add non-duplicate verification?)
        child = self.binddata.append(parent, [dest]);
     
    def _add_binding(self, widget, data=None):
        # input validation
        if( self.handlers.get_active() < 0 or self.emitters.get_active() < 0):
            print "invalid selections."
            return;

        # add to the tree model
        self._add_binding_impl(self.emitters.get_active_text(), self.handlers.get_active_text());
        self.treeview.expand_all();

    def _rem_binding(self, widget, data=None):
        (model,iter) = self.treeview.get_selection().get_selected();
        self.binddata.remove(iter);

    def _update_gui(self, *args):
        """
        Repopulate the parameters box (if changed).
        Update all the input parameters.
        Update the error messages box.
        Hide the box if there are no errors.
        Update the documentation block.
        Hide the box if there are no docs.
        """

#        try:
#            print "existing bindings"
#            print self._block.get_param("bindings");
#        except:
#            pass

        emitter_db = self.get_known_emitters();
        handler_db = self.get_known_handlers();

        emitter_names = map(lambda x:"%s"%(x[2]), emitter_db);
        handler_names = map(lambda x:"%s"%(x[0]), handler_db);
        #emitter_names = map(lambda x:"%s::%s"%(x[0], x[2]), emitter_db);

        self._bindings_box.hide_all()
        
        bbh = gtk.HBox();

        # set up emitter and handler selection
        self.emitters = gtk.combo_box_new_text();
        for i in emitter_names:
            self.emitters.append_text(i);
        self.handlers = gtk.combo_box_new_text();
        for i in handler_names:
            self.handlers.append_text(i);
        self._bindings_box.pack_start(bbh);
        bbh.pack_start(self.emitters);
        bbh.pack_start(self.handlers);


        # set up buttons
        bbh2 = gtk.HBox();
        self.addbut = gtk.Button("^ Add Binding ^");
        self.rembut = gtk.Button("v Remove Binding v");
        self.addbut.connect("clicked", self._add_binding, None);
        self.rembut.connect("clicked", self._rem_binding, None);
        bbh2.pack_start(self.addbut);
        bbh2.pack_start(self.rembut);
        self._bindings_box.pack_start(bbh2);

        
        # set up event bindings view
        self.binddata = gtk.TreeStore(str)
        self.treeview = gtk.TreeView(self.binddata)
        col = gtk.TreeViewColumn('Current Bindings');
        self.treeview.append_column(col);
        crt = gtk.CellRendererText();
        col.pack_start(crt, True);
        col.add_attribute(crt, 'text', 0);
        self._bindings_box.pack_start(self.treeview);


        # populate known binding data.
        try:
            for bp in self._block._bindings:
                self._add_binding_impl( bp[0], bp[1] );    
        except:
            pass
        self.treeview.expand_all();

        self._bindings_box.show_all()

        #update the errors box
        if self._block.is_valid(): self._error_box.hide()
        else: self._error_box.show()
        messages = '\n\n'.join(self._block.get_error_messages())
        self._error_messages_text_display.set_text(messages)

    def _handle_key_press(self, widget, event):
        """
        Handle key presses from the keyboard.
        Call the ok response when enter is pressed.
        @return false to forward the keypress
        """
        if event.keyval == gtk.keysyms.Return:
            self.response(gtk.RESPONSE_ACCEPT)
            return True #handled here
        return False #forward the keypress

    def run(self):
        """
        Run the dialog and get its response.
        @return true if the response was accept
        """
        response = gtk.Dialog.run(self)
        if(response == gtk.RESPONSE_ACCEPT):
            print "setting up bindings"
            nb = list();

            i = self.binddata.get_iter_first();
            while(i != None):
                source = self.binddata.get_value(i, 0);
                j = self.binddata.iter_children(i);
                while(j != None):
                    dest = self.binddata.get_value(j,0);
                    nb.append( (source,dest) );
                    j = self.binddata.iter_next(j);
                i = self.binddata.iter_next(i);
            self._block._bindings = nb;
        else:
            pass;
#            print "Not setting bindings"

        self.destroy()
        return response == gtk.RESPONSE_ACCEPT
