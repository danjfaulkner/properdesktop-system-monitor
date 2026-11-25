// SPDX-FileCopyrightText: 2026 Reion Wong <reionwong@gmail.com>
// SPDX-License-Identifier: BSD-3-Clause

#pragma once

#include "ui/geometry.h"
#include "ui/label.h"
#include "ui/layout.h"
#include "ui/painter.h"
#include "ui/cell_layout.h"
#include "ui/view.h"

#include <optional>
#include <string>

// FishUI's point sizes in logical pixels.
inline double points(double size)
{
    return size * 4 / 3;
}

// The colour laid over the base, as Qt.tint does.
properdesktop::ui::Color tint(properdesktop::ui::Color base, properdesktop::ui::Color overlay);

properdesktop::ui::Label* addLabel(properdesktop::ui::View* parent, std::string text, double pointSize = 0, int weight = 400,
                              std::optional<properdesktop::ui::Color> color = std::nullopt);

// A rounded plate in a colour, its corners the theme's unless told otherwise.
class Plate : public properdesktop::ui::View
{
public:
    explicit Plate(double radius = 0);

    void setRadius(double radius);
    // Unset: the theme's second background.
    void setColor(std::optional<properdesktop::ui::Color> color);
    void setContinuousCorners(std::optional<bool> continuous);

protected:
    void paint(properdesktop::ui::Painter& painter) override;

    properdesktop::ui::Corners corners() const;

private:
    double m_radius;
    std::optional<properdesktop::ui::Color> m_color;
    std::optional<bool> m_continuous;
};

// One device pixel at any scale, across the bottom of its rect.
void paintSeparator(properdesktop::ui::Painter& painter, double x, double bottom, double width);

// One read-only row of a ListCard: the label, then its value in secondary text.
class InfoRow : public properdesktop::ui::View
{
public:
    InfoRow(std::string text = {}, std::string value = {});

    void setText(std::string text);
    void setValue(std::string value);
    void setValueElide(properdesktop::ui::Label::Elide elide);
    // Drawn below every visible row but the card's last.
    void setSeparatorVisible(bool visible);

    properdesktop::ui::SizeF sizeHint() const override { return {0, 44}; }

protected:
    void layout() override;
    void paint(properdesktop::ui::Painter& painter) override;

private:
    properdesktop::ui::RowLayout* m_row;
    properdesktop::ui::Label* m_text;
    properdesktop::ui::Label* m_value;
    bool m_separator = false;
};

// A card of InfoRows: no inner padding, the rows carry their own.
class ListCard : public Plate
{
public:
    ListCard();

    InfoRow* addRow(std::string text, std::string value = {});
    // After rows were shown or hidden.
    void updateSeparators();

    properdesktop::ui::SizeF sizeHint() const override;
    double heightForWidth(double width) const override;

protected:
    void layout() override;
};

// Names the card below it; padded to the card's text edge.
class SectionLabel : public properdesktop::ui::View
{
public:
    explicit SectionLabel(std::string text = {});

    void setText(std::string text);
    void setElide(properdesktop::ui::Label::Elide elide);

    properdesktop::ui::SizeF sizeHint() const override;
    double heightForWidth(double width) const override;

protected:
    void layout() override;

private:
    properdesktop::ui::Label* m_label;
};
