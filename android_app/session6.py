from kivy.app import App
from kivy.uix.floatlayout import FloatLayout
from kivy.uix.widget import Widget
from kivy.uix.button import Button
from kivy.uix.label import Label
from kivy.properties import ObjectProperty
from kivy.graphics import Color, Rectangle



class deli_class(Widget):
    # btn = ObjectProperty(None)

    def __init__(self, **kwargs):
        super(deli_class, self).__init__(**kwargs)
        with self.canvas:
            self.label1 = Label(text="HI MY BABY :)", pos=(200, 300))
            self.b1 = Button(text="ok")
            self.b1.bind(on_press=self.callback, on_release=self.release_callback)
            self.add_widget(self.label1)
            self.add_widget(self.b1)

            Color(1, 0, 0, 0.5, mode='rgba')
            self.rect = Rectangle(pos=(100, 100), size=(250, 120))

        

    def on_touch_move(self, touch):
        self.rect.pos = touch.pos
        # print("move: ", touch)

    def callback(self, instance):
        self.label1.text = "hi my baby <3"
    
    def release_callback(self, instance):
        self.label1.text = "HI MY BABY :)"
    



class myApplicationnn(App):
    def build(self):
        return deli_class()


if __name__ == "__main__":
    myApplicationnn().run()
