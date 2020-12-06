from django.db import models

# Create your models here.

class HybridClassroom(models.Model):
    # session id for each test
    session_id = models.AutoField(auto_created=True,
                                  primary_key=True,
                                  serialize=False,
                                  verbose_name='ID')

    # Bear color
    color_red = models.IntegerField(default=0)
    color_green = models.IntegerField(default=0)
    color_blue = models.IntegerField(default=0)

    # Student raised hand: will be false unless hand is raised
    hand_raised = models.BooleanField(default=False)

    # Teacher check for understanding: will be null, true, or false
    understanding = models.BooleanField(null=True)

    # Student giving a round of applause: either true or false
    applause = models.BooleanField(default=False)

    # Teacher initiated a high five: will be true or false
    high_five = models.BooleanField(default=False)

    # Negative feedback
    negative = models.BooleanField(default=False)


    # String representation
    def __str__(self):
        return f"Session {self.session_id} with:\n" + \
            f"RGB: ({self.color_red}, {self.color_green}, {self.color_blue})\n" + \
            f"Hand raised: {self.hand_raised}\n" + \
            f"Understanding: {self.understanding}\n" + \
            f"Applause: {self.applause}\n" + \
            f"High five: {self.high_five}"
