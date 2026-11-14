from copy import deepcopy
from io import BytesIO
from pathlib import Path
import hashlib
from docx import Document
from docx.shared import Cm, Pt, RGBColor
from docx.enum.text import WD_ALIGN_PARAGRAPH, WD_BREAK
from docx.enum.table import WD_TABLE_ALIGNMENT, WD_CELL_VERTICAL_ALIGNMENT
from docx.oxml import OxmlElement
from docx.oxml.ns import qn

ROOT = Path(__file__).resolve().parent
SOURCE = ROOT / 'source.docx'
OUTPUT = ROOT / '人工智能概论_第一次作业_排版优化版.docx'
doc = Document(SOURCE)
table = doc.tables[0]

def font(run, size=10.5, bold=False):
    run.font.name = 'Times New Roman'
    run.font.size = Pt(size)
    run.font.bold = bold
    run.font.color.rgb = RGBColor(0, 0, 0)
    run.font.underline = False
    props = run._element.get_or_add_rPr()
    fonts = props.rFonts
    fonts.set(qn('w:eastAsia'), '宋体')

def paragraph(p, center=False, heading=False):
    pf = p.paragraph_format
    pf.alignment = WD_ALIGN_PARAGRAPH.CENTER if center else WD_ALIGN_PARAGRAPH.JUSTIFY
    pf.space_before = Pt(4 if heading else 0)
    pf.space_after = Pt(3)
    pf.line_spacing = Pt(14)
    pf.first_line_indent = Pt(0 if center or heading else 21)
    pf.left_indent = Pt(0)
    pf.right_indent = Pt(0)
    pf.keep_with_next = heading
    pf.keep_together = False
    pf.widow_control = True
    pf.page_break_before = False
    for run in p.runs:
        font(run, bold=heading)
    for tag in ('spacing', 'ind', 'contextualSpacing', 'snapToGrid'):
        # Explicit paragraph formatting above takes priority over legacy spacing.
        if tag in ('contextualSpacing', 'snapToGrid'):
            for item in list(p._p.get_or_add_pPr().findall(qn('w:'+tag))):
                p._p.pPr.remove(item)

# Retain the report's title, identification, table labels, contents and images.
section = doc.sections[0]
section.page_width, section.page_height = Cm(21), Cm(29.7)
section.top_margin, section.bottom_margin = Cm(1.7), Cm(1.7)
section.left_margin, section.right_margin = Cm(1.8), Cm(1.8)
for grid in section._sectPr.findall(qn('w:docGrid')):
    section._sectPr.remove(grid)
for style_name in ('Normal', 'Normal (Web)'):
    if style_name in doc.styles:
        style = doc.styles[style_name]
        style.font.name = 'Times New Roman'
        style.font.size = Pt(10.5)
        style.paragraph_format.space_after = Pt(3)
        style.paragraph_format.line_spacing = Pt(14)

title = doc.paragraphs[0]
title.text = '实 验 报 告'
if 'Title' not in doc.styles:
    from docx.enum.style import WD_STYLE_TYPE
    doc.styles.add_style('Title', WD_STYLE_TYPE.PARAGRAPH)
title.style = doc.styles['Title']
paragraph(title, center=True)
title.paragraph_format.space_after = Pt(10)
title.paragraph_format.line_spacing = Pt(25)
for run in title.runs:
    font(run, 20, True)
    run._element.rPr.rFonts.set(qn('w:eastAsia'), '黑体')
meta = doc.paragraphs[1]
paragraph(meta, center=True)
meta.paragraph_format.space_after = Pt(8)
for run in meta.runs:
    font(run, 10)

# Save embedded images before replacing legacy floating VML objects.
result_cell = table.rows[3].cells[1]
images = []
for p in result_cell.paragraphs:
    for im in p._p.xpath('.//*[local-name()="imagedata"]'):
        rid = im.get(qn('r:id'))
        images.append(doc.part.related_parts[rid].blob)
assert len(images) == 4, len(images)
result_texts = [p.text for p in result_cell.paragraphs]

# Remove empty spacer paragraphs; split the run-on heading into two paragraphs.
flow_cell = table.rows[2].cells[1]
for p in list(flow_cell.paragraphs):
    if not p.text.strip():
        p._p.getparent().remove(p._p)
    elif p.text.startswith('3.训练基础模型。'):
        text = p.text[len('3.训练基础模型。'):]
        p.text = '3.训练基础模型'
        node = OxmlElement('w:p')
        p._p.addnext(node)
        from docx.text.paragraph import Paragraph
        Paragraph(node, p._parent).add_run(text)
for p in flow_cell.paragraphs:
    paragraph(p, heading=p.text.startswith(('1.', '2.', '3.', '4.')))

# Reuse the original table cells, in a second table beginning on the next page.
second_xml = deepcopy(table._tbl)
for tr in list(second_xml.findall(qn('w:tr')))[:3]:
    second_xml.remove(tr)
for tr in list(table._tbl.findall(qn('w:tr')))[3:]:
    table._tbl.remove(tr)
break_p = OxmlElement('w:p')
table._tbl.addnext(break_p)
from docx.text.paragraph import Paragraph
break_para = Paragraph(break_p, doc._body)
break_para.add_run().add_break(WD_BREAK.PAGE)
break_para.paragraph_format.space_after = Pt(0)
break_para.paragraph_format.space_before = Pt(0)
break_para.paragraph_format.line_spacing = Pt(1)
break_p.addnext(second_xml)
from docx.table import Table
second_table = Table(second_xml, doc._body)
result_cell = second_table.rows[0].cells[1]
for p in list(result_cell.paragraphs):
    p._p.getparent().remove(p._p)

def add_text(text, caption=False):
    p = result_cell.add_paragraph(text)
    paragraph(p, center=caption)
    if caption:
        p.paragraph_format.keep_with_next = False
        for r in p.runs:
            font(r, 9)
    return p

def add_image(blob, width):
    p = result_cell.add_paragraph()
    paragraph(p, center=True)
    p.paragraph_format.line_spacing = 1
    p.paragraph_format.space_before = Pt(2)
    p.paragraph_format.space_after = Pt(2)
    p.add_run().add_picture(BytesIO(blob), width=Cm(width))

add_text(result_texts[0])
add_image(images[0], 8.2)
add_text(result_texts[1])
add_text(result_texts[2])
add_image(images[1], 8.2)
add_image(images[2], 12.0)
add_image(images[3], 12.0)
add_text(result_texts[7], caption=True)
add_text(result_texts[8])

for ti, t in enumerate(doc.tables):
    t.alignment = WD_TABLE_ALIGNMENT.CENTER
    t.autofit = False
    # Original merged cells retained; widen to the A4 text area.
    widths = [2.5, 7.5, 2.3, 5.1]
    for col, width in zip(t.columns, widths):
        col.width = Cm(width)
    for row in t.rows:
        trpr = row._tr.get_or_add_trPr()
        for node in list(trpr):
            if node.tag in (qn('w:trHeight'), qn('w:cantSplit')):
                trpr.remove(node)
        seen = set()
        for ci, cell in enumerate(row.cells):
            if cell._tc in seen:
                continue
            seen.add(cell._tc)
            span = cell._tc.grid_span
            cell.width = Cm(sum(widths[ci:ci+span]))
            cell.vertical_alignment = WD_CELL_VERTICAL_ALIGNMENT.TOP
            tcpr = cell._tc.get_or_add_tcPr()
            margins = tcpr.find(qn('w:tcMar'))
            if margins is None:
                margins = OxmlElement('w:tcMar')
                tcpr.append(margins)
            for side, value in [('top', 70), ('bottom', 70), ('left', 100), ('right', 100)]:
                node = margins.find(qn('w:'+side))
                if node is None:
                    node = OxmlElement('w:'+side)
                    margins.append(node)
                node.set(qn('w:w'), str(value))
                node.set(qn('w:type'), 'dxa')
            for p in cell.paragraphs:
                is_pic = bool(p._p.xpath('.//w:drawing'))
                if not is_pic:
                    paragraph(p, center=(ci == 0 or (ti == 0 and row._tr in [t.rows[0]._tr,t.rows[1]._tr])),
                              heading=p.text.startswith(('1.', '2.', '3.', '4.')))
                if ci == 0:
                    for r in p.runs:
                        font(r, 10.5, True)

# Remove the original trailing spacers, retaining one mandatory paragraph.
for p in list(doc.paragraphs)[2:]:
    if p._p is not break_p and not p.text.strip() and not p._p.xpath('.//w:br'):
        p._p.getparent().remove(p._p)
doc.add_paragraph().paragraph_format.space_after = Pt(0)
doc.save(OUTPUT)
print(OUTPUT)
print('Embedded images:', len(images))
